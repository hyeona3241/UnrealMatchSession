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
    IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
    if (!OSS) return;
    auto SessInt = OSS->GetSessionInterface();
    if (!SessInt.IsValid()) return;

    // Delegate ���
    SessInt->AddOnCreateSessionCompleteDelegate_Handle(
        FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)
    );

    // ����
    FOnlineSessionSettings Settings;
    Settings.bIsLANMatch = false;
    Settings.NumPublicConnections = 4;
    Settings.bShouldAdvertise = true;
    Settings.Set(FName("MatchType"), FString("FreeForAll"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

    // ���� ���� ����
    int32 HostingPlayerIndex = 0;

    SessInt->CreateSession(
        HostingPlayerIndex,
        NAME_GameSession,
        Settings
    );

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

        // ���� ���� ����, ������ �κ� ������ �̵�
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
