// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API ALobbyPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

private:
    // Replicated GameState ���� �� �̸��� �о�� ȭ�鿡 ����
    void ClientLogSelectedMap();
	
};
