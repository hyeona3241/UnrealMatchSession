// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystemGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MENUSYSTEM_API UMenuSystemGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LaunchDedicatedServer();


private:
	bool bIsHostClient = false;  // 호스트 클라이언트 여부

	// 서버가 실행된 후 3초 뒤에 클라이언트가 자동으로 로비로 이동
	void AutoJoinAfterServerBoot();
	
};
