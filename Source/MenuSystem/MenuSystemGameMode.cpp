// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuSystemGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystemCharacter.h"
#include "Engine/Engine.h"

AMenuSystemGameMode::AMenuSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}


void AMenuSystemGameMode::CreateSessionIfServer()
{
    IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
    if (!Subsystem) return;

    IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
    if (!SessionInterface.IsValid()) return;

    FOnlineSessionSettings SessionSettings;
    SessionSettings.bIsLANMatch = false;
    SessionSettings.NumPublicConnections = 4;
    SessionSettings.bAllowJoinInProgress = true;
    SessionSettings.bAllowJoinViaPresence = true;
    SessionSettings.bShouldAdvertise = true;
    SessionSettings.bUsesPresence = true;
    SessionSettings.bUseLobbiesIfAvailable = true;
    SessionSettings.Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    // ���� ���� �� �ݹ� �Լ� ���ε�
    FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
    CreateSessionCompleteDelegate.BindUObject(this, &AMenuSystemGameMode::OnCreateSessionComplete);

    // ���� ���� ȣ��
    SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString::Printf(TEXT("Creating Session!"))
        );
    }
}

void AMenuSystemGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld()->GetNetMode() == NM_DedicatedServer)
    {
        UE_LOG(LogTemp, Log, TEXT("Dedicated server GameMode BeginPlay"));
        CreateSessionIfServer();
    }
}

void AMenuSystemGameMode::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
    if (bWasSuccessful)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Blue,
                FString::Printf(TEXT("Created session: %s"), *SessionName.ToString())
            );
        }

        // ������ Ŭ���̾�Ʈ���� ������ �����ϰ� �κ�� �̵�
        UWorld* World = GetWorld();
        if (World)
        {
            // �� ������� ������ �κ�� �̵�
            World->ServerTravel(FString("/Game/ThirdPerson/Maps/Lobby?listen"));
        }
    }
    else
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                15.f,
                FColor::Red,
                FString(TEXT("Failed to create session!"))
            );
        }
    }
}
