// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "LobbyGameState.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

void ALobbyPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // BeginPlay 직후 GameState의 Replication 준비를 위해 딜레이
    FTimerHandle TempHandle;
    GetWorldTimerManager().SetTimer(
        TempHandle,
        this,
        &ALobbyPlayerController::ClientLogSelectedMap,
        1.0f, 
        false  
    );
}

void ALobbyPlayerController::ClientLogSelectedMap()
{
    if (ALobbyGameState* GS = GetWorld()->GetGameState<ALobbyGameState>())
    {
        // SelectedMap 이 아직 빈 문자열이 아닐 때만 출력
        if (!GS->SelectedMap.IsEmpty() && GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                5.0f,
                FColor::Green,
                FString::Printf(TEXT("Map: %s"), *GS->SelectedMap)
            );
        }
    }
}

