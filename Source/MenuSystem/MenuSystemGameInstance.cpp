#include "MenuSystemGameInstance.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"
#include "MenuSystemCharacter.h"
#include "MenuSystemGameMode.h"

void UMenuSystemGameInstance::Init()
{
    Super::Init();
    UE_LOG(LogTemp, Log, TEXT("Custom GameInstance Initialized!"));
}

void UMenuSystemGameInstance::LaunchDedicatedServer()
{
#if PLATFORM_WINDOWS
    FString ProjectDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectDir());
    FString ExecutablePath = FPaths::Combine(ProjectDir, TEXT("Binaries/Win64/MenuSystemServer.exe"));
    FString MapPath = TEXT("/Game/ThirdPerson/Maps/Lobby");
    FString Params = FString::Printf(TEXT("%s -log -nosteam -port=7777"), *MapPath);

    FPlatformProcess::CreateProc(
        *ExecutablePath,
        *Params,
        true, false, false, nullptr, 0, nullptr, nullptr
    );

    UE_LOG(LogTemp, Log, TEXT("Dedicated server launched from client!"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Blue,
            FString::Printf(TEXT("Creating Dedicated Server"))
        );
    }

    // 서버가 로비로 이동하도록 처리
    UWorld* World = GetWorld();
    if (World /*&& World->GetNetMode() == NM_DedicatedServer*/) // 서버 여부 확인
    {
        // 서버가 시작된 후 세션 생성 함수 호출
        AMenuSystemGameMode* GameMode = Cast<AMenuSystemGameMode>(World->GetAuthGameMode());
        if (GameMode)
        {
            // 세션 생성
            GameMode->CreateSessionIfServer();
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(
                    -1,
                    15.f,
                    FColor::Magenta,
                    FString::Printf(TEXT("Creating Session...."))
                );
            }
        }

        // 서버 시작 후, 자동으로 로비에 접속하도록 3초 뒤에 호출
        bIsHostClient = true;
        FTimerHandle TempHandle;
        GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UMenuSystemGameInstance::AutoJoinAfterServerBoot, 3.0f, false);
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Magenta,
                FString::Printf(TEXT("no Server"))
            );
        }
    }
#endif
}

void UMenuSystemGameInstance::AutoJoinAfterServerBoot()
{
    if (!bIsHostClient) return;

    // 캐릭터 찾기
    APlayerController* PC = GetFirstLocalPlayerController();
    if (!PC) return;

    AMenuSystemCharacter* Character = Cast<AMenuSystemCharacter>(PC->GetPawn());
    if (Character)
    {
        Character->JoinGameSession();

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Yellow,
                FString::Printf(TEXT("Join Match Session Host"))
            );
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character not ready yet, retrying..."));
        // 캐릭터 스폰이 늦었을 경우를 대비해 재시도
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &UMenuSystemGameInstance::AutoJoinAfterServerBoot, 1.0f, false);
    }
}
