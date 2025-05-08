#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectWidget.h" 
#include "MainMenuWidget.generated.h"

UCLASS()
class MENUSYSTEM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// ���� ����⡱ ��ư
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateRoom;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinRoom;

	// �������Ʈ���� ������ MapSelectWidget Ŭ����
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<UMapSelectWidget> MapSelectWidgetClass;

private:
	// Ŭ�� �ڵ鷯
	UFUNCTION()
	void OnCreateRoomPressed();

	UFUNCTION()
	void OnJoinRoomPressed();

	// MapSelectWidget �� ��������Ʈ �ݹ�
	UFUNCTION()
	void HandleMapConfirmed(const FString& MapName);
};
