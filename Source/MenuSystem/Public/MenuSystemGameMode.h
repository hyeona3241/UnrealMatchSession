// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuSystemGameMode.generated.h"

UCLASS(minimalapi)
class AMenuSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMenuSystemGameMode();
	void CreateSessionIfServer();

protected:
	virtual void BeginPlay() override;

	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
};



