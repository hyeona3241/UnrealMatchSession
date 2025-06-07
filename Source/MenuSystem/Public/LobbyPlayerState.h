// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALobbyPlayerState();

	// ReplicatedUsing 으로 지정
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSlot, BlueprintReadOnly, Category = "Lobby")
	int32 PlayerSlot;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Lobby")
	FString PlayerUID;

	UFUNCTION()
	void OnRep_PlayerSlot();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
	
