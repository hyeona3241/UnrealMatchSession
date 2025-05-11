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

	void OnMapLoaded(UWorld* LoadedWorld);

	UFUNCTION(BlueprintCallable)
	void LaunchDedicatedServer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Session")
	FString DesiredMap;


private:
	bool bIsHostClient = false;  // ȣ��Ʈ Ŭ���̾�Ʈ ����

	// ������ ����� �� 3�� �ڿ� Ŭ���̾�Ʈ�� �ڵ����� �κ�� �̵�
	void AutoJoinAfterServerBoot();

	UPROPERTY()
	UUserWidget* MainMenuWidgetInstance = nullptr;
	
};
