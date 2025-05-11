// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "Net/UnrealNetwork.h"

ALobbyGameState::ALobbyGameState()
{
    SelectedMap = TEXT("");
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // SelectedMap ������ Replicate ������� ���
    DOREPLIFETIME(ALobbyGameState, SelectedMap);

}