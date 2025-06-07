// Copyright Epic Games, Inc. All Rights Reserved.

#include "MenuSystemGameMode.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MenuSystemCharacter.h"
#include "Engine/Engine.h"
#include <steam/steam_gameserver.h> 
#include "steam/steam_api.h"

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
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS) return;

    IOnlineSessionPtr SessionInt = OSS->GetSessionInterface();
    if (!SessionInt.IsValid()) return;

    // Delegate 등록
    SessionInt->AddOnCreateSessionCompleteDelegate_Handle(
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)
    );

    FOnlineSessionSettings Settings;
    Settings.bIsDedicated = true; 
    Settings.bIsLANMatch = false;
    Settings.NumPublicConnections = 4;
    Settings.bShouldAdvertise = true;
    Settings.bUsesPresence = false; 
    Settings.bAllowJoinInProgress = true;
    Settings.bAllowJoinViaPresence = false; 
    Settings.bUseLobbiesIfAvailable = false; 
    Settings.Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    int32 HostingPlayerIndex = 0;
    bool bSuccess = SessionInt->CreateSession(HostingPlayerIndex, NAME_GameSession, Settings);

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            15.f,
            FColor::Yellow,
            FString::Printf(TEXT("Creating Session! Success: %s"), bSuccess ? TEXT("Yes") : TEXT("No"))
        );
    }
}

void AMenuSystemGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld()->GetNetMode() == NM_DedicatedServer)
    {
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

        if (!bWasSuccessful) return;

        // 세션 만든 다음, 서버는 로비 맵으로 이동
        GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/Lobby?listen"));
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
