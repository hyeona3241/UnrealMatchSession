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

    // ���������� UI ���� ����
    if (LoadedWorld->GetNetMode() == NM_DedicatedServer)
        return;

    // �� �̸��� �κ�� UI ���� ����
    FString MapName = LoadedWorld->GetMapName();
    MapName.RemoveFromStart(LoadedWorld->StreamingLevelsPrefix); // UEDPIE ���ξ� ����

    if (MapName.Equals("Lobby") || MapName.Equals("LobbyMap")) // �κ� �� �̸��� �°� ����
        return;

    // ���� ���� �� �߰�
    UUserWidget* MainMenu = CreateWidget<UUserWidget>(LoadedWorld, MainMenuWidgetClass);
    if (!MainMenu) return;

    MainMenu->AddToViewport();

    // �÷��̾� ��Ʈ�ѷ� ��������
    APlayerController* PC = LoadedWorld->GetFirstPlayerController();
    if (PC)
    {
        // ���콺 Ŀ�� ǥ��
        PC->bShowMouseCursor = true;

        // �Է� ��� ����
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
    // 1) ���� ���� ���� EXE�� �ִ� ���� ���
    FString ExeDir = FPaths::GetPath(FPlatformProcess::ExecutablePath());
    // 2) �� ���� ���� ���� EXE �̸� (��Ű¡ �� ������ �� ���� exe)
    FString ServerExe = FPaths::Combine(ExeDir, TEXT("MenuSystemServer.exe"));
    // 3) ���� ���� �� + listen �÷���
    FString MapName = TEXT("/Game/ThirdPerson/Maps/Lobby?listen");
    // 4) ���� ���, ������ ����, ��Ʈ ����, �α�
    FString Params = FString::Printf(TEXT("%s -server -nullrhi -port=7777 -log"), *MapName);

    // 5) ���μ��� ����
    FProcHandle ProcHandle = FPlatformProcess::CreateProc(
        *ServerExe,
        *Params,
        true,   // bLaunchDetached
        false,  // bLaunchHidden
        false,  // bLaunchReallyHidden
        nullptr, 0, nullptr, nullptr
    );

    // 6) ����� �α�
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

    //--- ������ ���� ���� (���� ����, �ڵ� ����) �� �״�� ���� ---
    UWorld* World = GetWorld();
    if (World)
    {
        // ������ GameMode BeginPlay���� ���� �����ϵ���
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
