// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API ALobbyGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ALobbyGameState();

    //������ �� �̸�
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
    FString SelectedMap;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
