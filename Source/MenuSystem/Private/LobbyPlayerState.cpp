// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

ALobbyPlayerState::ALobbyPlayerState()
{
    PlayerSlot = 0;
    PlayerUID = TEXT("");
}

void ALobbyPlayerState::OnRep_PlayerSlot()
{

    UE_LOG(LogTemp, Log, TEXT("<<Slot Assigned>> PlayerSlot=%d, UID=%s"), PlayerSlot, *PlayerUID);

    if (APlayerController* PC = Cast<APlayerController>(GetOwner()))
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            5.0f,
            FColor::Green,
            FString::Printf(TEXT("Player%d : %s"), PlayerSlot, *PlayerUID)
        );
    }
}

void ALobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ALobbyPlayerState, PlayerSlot);
    DOREPLIFETIME(ALobbyPlayerState, PlayerUID);
}

