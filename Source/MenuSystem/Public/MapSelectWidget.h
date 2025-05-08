#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "MapSelectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMapConfirmed,     // 델리게이트 타입
    const FString&,      // 파라미터 타입
    MapName              // 파라미터 이름
);
/**
 * UMapSelectWidget
 * 타이틀 화면에서 맵을 3개 중 선택하고 Confirm을 누르면
 * SelectedMap 이름을 브로드캐스트합니다.
 */
UCLASS()
class MENUSYSTEM_API UMapSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // 위젯 초기화 시 바인딩
    virtual void NativeConstruct() override;

    // 선택된 맵 이름을 외부에 통보
    UPROPERTY(BlueprintAssignable, Category = "MapSelect")
    FOnMapConfirmed OnMapConfirmed;

protected:
    // 각 맵 아이콘을 감싸는 Border (테두리 강조용)
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map1;
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map2;
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map3;

    // 실제 클릭할 Button (맵 선택)
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map1;
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map2;
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map3;

    // 확인 버튼
    UPROPERTY(meta = (BindWidget)) UButton* Btn_Confirm;

private:
    // 현재 선택된 맵 이름 저장
    FString SelectedMap;

    // 클릭 핸들러
    UFUNCTION() void OnMap1Clicked();
    UFUNCTION() void OnMap2Clicked();
    UFUNCTION() void OnMap3Clicked();
    UFUNCTION() void OnConfirmClicked();

    // 어떤 Border를 강조할지 토글
    void HighlightSelection(UBorder* ChosenBorder);
};
