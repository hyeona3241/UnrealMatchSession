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
    if (!LoadedWorld || !MainMenuWidgetClass)
        return;

    // 서버에서는 UI 생성 안함
    if (LoadedWorld->GetNetMode() == NM_DedicatedServer)
        return;

    // 맵 이름이 로비면 UI 생성 안함
    FString MapName = LoadedWorld->GetMapName();
    MapName.RemoveFromStart(LoadedWorld->StreamingLevelsPrefix); // UEDPIE 접두어 제거

    if (MapName.Equals("Lobby") || MapName.Equals("LobbyMap")) // 로비 맵 이름에 맞게 수정
        return;

    // 위젯 생성 및 추가
    UUserWidget* MainMenu = CreateWidget<UUserWidget>(LoadedWorld, MainMenuWidgetClass);
    if (!MainMenu) return;

    MainMenu->AddToViewport();

    // 플레이어 컨트롤러 가져오기
    APlayerController* PC = LoadedWorld->GetFirstPlayerController();
    if (PC)
    {
        // 마우스 커서 표시
        PC->bShowMouseCursor = true;

        // 입력 모드 설정
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(MainMenu->TakeWidget());
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
