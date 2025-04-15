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
	bool bIsHostClient = false;  // ȣ��Ʈ Ŭ���̾�Ʈ ����

	// ������ ����� �� 3�� �ڿ� Ŭ���̾�Ʈ�� �ڵ����� �κ�� �̵�
	void AutoJoinAfterServerBoot();
	
};
