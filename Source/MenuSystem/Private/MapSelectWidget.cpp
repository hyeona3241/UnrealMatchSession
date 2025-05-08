#include "MapSelectWidget.h"
#include "Components/Button.h"
#include "Components/Border.h"

void UMapSelectWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // ��ư Ŭ�� �̺�Ʈ ���ε�
    if (MapBtn_Map1)  MapBtn_Map1->OnClicked.AddDynamic(this, &ThisClass::OnMap1Clicked);
    if (MapBtn_Map2)  MapBtn_Map2->OnClicked.AddDynamic(this, &ThisClass::OnMap2Clicked);
    if (MapBtn_Map3)  MapBtn_Map3->OnClicked.AddDynamic(this, &ThisClass::OnMap3Clicked);
    if (Btn_Confirm)  Btn_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);

    // �ʱ⿣ �ƹ� ������ ���� ����
    HighlightSelection(nullptr);
}

void UMapSelectWidget::OnMap1Clicked()
{
    SelectedMap = TEXT("map1");
    HighlightSelection(Border_Map1);
}

void UMapSelectWidget::OnMap2Clicked()
{
    SelectedMap = TEXT("map2");
    HighlightSelection(Border_Map2);
}

void UMapSelectWidget::OnMap3Clicked()
{
    SelectedMap = TEXT("map3");
    HighlightSelection(Border_Map3);
}

void UMapSelectWidget::OnConfirmClicked()
{
    if (!SelectedMap.IsEmpty())
    {
        // ���õ� �� �̸��� ��ε�ĳ��Ʈ
        OnMapConfirmed.Broadcast(SelectedMap);
        // UI �ݱ�
        RemoveFromParent();
    }
}

void UMapSelectWidget::HighlightSelection(UBorder* ChosenBorder)
{
    // ��� �׵θ��� �������� �ʱ�ȭ
    if (Border_Map1) Border_Map1->SetBrushColor(FLinearColor::Transparent);
    if (Border_Map2) Border_Map2->SetBrushColor(FLinearColor::Transparent);
    if (Border_Map3) Border_Map3->SetBrushColor(FLinearColor::Transparent);

    // ���õ� �׵θ��� ���� �� ����
    if (ChosenBorder)
    {
        const FLinearColor HighlightColor(1.f, 0.8f, 0.f, 0.5f);
        ChosenBorder->SetBrushColor(HighlightColor);
    }
}
