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
	// “방 만들기” 버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateRoom;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinRoom;

	// 블루프린트에서 지정할 MapSelectWidget 클래스
	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<UMapSelectWidget> MapSelectWidgetClass;

private:
	// 클릭 핸들러
	UFUNCTION()
	void OnCreateRoomPressed();

	UFUNCTION()
	void OnJoinRoomPressed();

	// MapSelectWidget 의 델리게이트 콜백
	UFUNCTION()
	void HandleMapConfirmed(const FString& MapName);
};
