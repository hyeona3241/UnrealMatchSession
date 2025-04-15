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

    // ������ �κ�� �̵��ϵ��� ó��
    UWorld* World = GetWorld();
    if (World /*&& World->GetNetMode() == NM_DedicatedServer*/) // ���� ���� Ȯ��
    {
        // ������ ���۵� �� ���� ���� �Լ� ȣ��
        AMenuSystemGameMode* GameMode = Cast<AMenuSystemGameMode>(World->GetAuthGameMode());
        if (GameMode)
        {
            // ���� ����
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

        // ���� ���� ��, �ڵ����� �κ� �����ϵ��� 3�� �ڿ� ȣ��
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

    // ĳ���� ã��
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
        // ĳ���� ������ �ʾ��� ��츦 ����� ��õ�
        FTimerHandle RetryHandle;
        GetWorld()->GetTimerManager().SetTimer(RetryHandle, this, &UMenuSystemGameInstance::AutoJoinAfterServerBoot, 1.0f, false);
    }
}
