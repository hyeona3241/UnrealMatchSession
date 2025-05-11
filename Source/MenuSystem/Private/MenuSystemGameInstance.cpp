#include "MenuSystemGameInstance.h"
#include "HAL/PlatformProcess.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"
#include "MenuSystemCharacter.h"
#include "MenuSystemGameMode.h"
#include "Blueprint/UserWidget.h"

void UMenuSystemGameInstance::Init()
{
    Super::Init();
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnMapLoaded);
}

void UMenuSystemGameInstance::OnMapLoaded(UWorld* LoadedWorld)
{
    if (!LoadedWorld)
        return;

    // 전용 서버는 UI 로직 자체 건너뛸 것
    if (LoadedWorld->GetNetMode() == NM_DedicatedServer)
        return;

    // 맵 이름에서 PIE 접두어 지우기
    FString MapName = LoadedWorld->GetMapName();
    MapName.RemoveFromStart(LoadedWorld->StreamingLevelsPrefix);

    // --- 1) 로비 맵일 때 ---
    if (MapName.Equals(TEXT("Lobby")) || MapName.Equals(TEXT("LobbyMap")))
    {
        // 화면에 붙어 있던 메인 메뉴 위젯 제거
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->RemoveFromParent();
            MainMenuWidgetInstance = nullptr;
        }

        // 완전 게임 모드로 전환 (커서 숨기고 입력도 게임만)
        if (APlayerController* PC = LoadedWorld->GetFirstPlayerController())
        {
            PC->bShowMouseCursor = false;
            PC->SetInputMode(FInputModeGameOnly());
        }
        return;
    }

    // --- 2) 그 외(메인 메뉴 등) 맵일 때 ---
    if (!MainMenuWidgetClass)
        return;

    // 이미 붙어 있던 게 있으면 지우고 새로 붙이기
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    // 위젯 생성
    MainMenuWidgetInstance = CreateWidget<UUserWidget>(LoadedWorld, MainMenuWidgetClass);
    if (!MainMenuWidgetInstance)
        return;

    MainMenuWidgetInstance->AddToViewport();

    // 커서 보이기 + GameAndUI 입력 모드
    if (APlayerController* PC = LoadedWorld->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC->SetInputMode(InputMode);
    }
}


void UMenuSystemGameInstance::LaunchDedicatedServer()
{
#if PLATFORM_WINDOWS
    // 1) 현재 실행 중인 EXE가 있는 폴더 경로
    FString ExeDir = FPaths::GetPath(FPlatformProcess::ExecutablePath());
    // 2) 그 폴더 안의 서버 EXE 이름 (패키징 때 복사해 둔 서버 exe)
    FString ServerExe = FPaths::Combine(ExeDir, TEXT("MenuSystemServer.exe"));
    // 3) 열고 싶은 맵 + listen 플래그
    FString MapName = TEXT("/Game/ThirdPerson/Maps/Lobby?listen");
    // 4) 서버 모드, 렌더링 생략, 포트 지정, 로그
    FString Params = FString::Printf(TEXT("%s -server -nullrhi -port=7777 -log"), *MapName);

    // 5) 프로세스 생성
    FProcHandle ProcHandle = FPlatformProcess::CreateProc(
        *ServerExe,
        *Params,
        true,   // bLaunchDetached
        false,  // bLaunchHidden
        false,  // bLaunchReallyHidden
        nullptr, 0, nullptr, nullptr
    );

    // 6) 디버그 로그
    UE_LOG(LogTemp, Log, TEXT("LaunchDedicatedServer() called"));
    UE_LOG(LogTemp, Log, TEXT("  Executable: %s"), *ServerExe);
    UE_LOG(LogTemp, Log, TEXT("  Params    : %s"), *Params);
    UE_LOG(LogTemp, Log, TEXT("  Proc valid? %s"), ProcHandle.IsValid() ? TEXT("Yes") : TEXT("No"));

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1, 10.f, FColor::Blue,
            FString::Printf(TEXT("Dedicated Server Started: %s"), ProcHandle.IsValid() ? TEXT("OK") : TEXT("FAILED"))
        );
    }

    //--- 나머지 기존 로직 (세션 생성, 자동 접속) 은 그대로 유지 ---
    UWorld* World = GetWorld();
    if (World)
    {
        // 서버가 GameMode BeginPlay에서 세션 생성하도록
        AMenuSystemGameMode* GM = Cast<AMenuSystemGameMode>(World->GetAuthGameMode());
        if (GM)
        {
            GM->CreateSessionIfServer();
            UE_LOG(LogTemp, Log, TEXT("Triggering CreateSessionIfServer()"));
        }

        bIsHostClient = true;
        FTimerHandle TempHandle;
        World->GetTimerManager().SetTimer(
            TempHandle,
            this,
            &UMenuSystemGameInstance::AutoJoinAfterServerBoot,
            3.0f,
            false
        );
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
