#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "MapSelectWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnMapConfirmed,     // ��������Ʈ Ÿ��
    const FString&,      // �Ķ���� Ÿ��
    MapName              // �Ķ���� �̸�
);
/**
 * UMapSelectWidget
 * Ÿ��Ʋ ȭ�鿡�� ���� 3�� �� �����ϰ� Confirm�� ������
 * SelectedMap �̸��� ��ε�ĳ��Ʈ�մϴ�.
 */
UCLASS()
class MENUSYSTEM_API UMapSelectWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // ���� �ʱ�ȭ �� ���ε�
    virtual void NativeConstruct() override;

    // ���õ� �� �̸��� �ܺο� �뺸
    UPROPERTY(BlueprintAssignable, Category = "MapSelect")
    FOnMapConfirmed OnMapConfirmed;

protected:
    // �� �� �������� ���δ� Border (�׵θ� ������)
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map1;
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map2;
    UPROPERTY(meta = (BindWidget)) UBorder* Border_Map3;

    // ���� Ŭ���� Button (�� ����)
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map1;
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map2;
    UPROPERTY(meta = (BindWidget)) UButton* MapBtn_Map3;

    // Ȯ�� ��ư
    UPROPERTY(meta = (BindWidget)) UButton* Btn_Confirm;

private:
    // ���� ���õ� �� �̸� ����
    FString SelectedMap;

    // Ŭ�� �ڵ鷯
    UFUNCTION() void OnMap1Clicked();
    UFUNCTION() void OnMap2Clicked();
    UFUNCTION() void OnMap3Clicked();
    UFUNCTION() void OnConfirmClicked();

    // � Border�� �������� ���
    void HighlightSelection(UBorder* ChosenBorder);
};
