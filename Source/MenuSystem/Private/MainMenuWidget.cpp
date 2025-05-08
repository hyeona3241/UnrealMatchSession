#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystemGameInstance.h"
#include <MenuSystemCharacter.h>

void UMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("UMainMenuWidget::NativeConstruct() called, Btn_CreateRoom=%s"),
        Btn_CreateRoom ? TEXT("Valid") : TEXT("Null"));

    if (Btn_CreateRoom)
        Btn_CreateRoom->OnClicked.AddDynamic(this, &ThisClass::OnCreateRoomPressed);

    if (Btn_JoinRoom)
        Btn_JoinRoom->OnClicked.AddDynamic(this, &ThisClass::OnJoinRoomPressed);

}

void UMainMenuWidget::OnCreateRoomPressed()
{
    if (!MapSelectWidgetClass) return;

    UMapSelectWidget* MapW = CreateWidget<UMapSelectWidget>(GetWorld(), MapSelectWidgetClass);
    if (MapW)
    {
        MapW->AddToViewport();
        MapW->OnMapConfirmed.AddDynamic(this, &ThisClass::HandleMapConfirmed);

        // 본 위젯(MainMenu)을 제거
        this->RemoveFromParent();
    }
}

void UMainMenuWidget::HandleMapConfirmed(const FString& MapName)
{
    // 1) GameInstance 에 선택 맵 저장
    if (auto* GI = Cast<UMenuSystemGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        GI->DesiredMap = MapName;
        // 2) Listen 서버 용으로 캐릭터에게 세션 생성 트리거
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            if (auto* Ch = Cast<AMenuSystemCharacter>(PC->GetPawn()))
            {
                Ch->CreateGameSession();
            }
        }
        // 혹은 GI->LaunchDedicatedServer();
    }
}

void UMainMenuWidget::OnJoinRoomPressed()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC) return;

    AMenuSystemCharacter* Ch = Cast<AMenuSystemCharacter>(PC->GetPawn());
    if (Ch)
    {
        Ch->JoinGameSession();

        // 본 위젯(MainMenu)을 제거
        this->RemoveFromParent();
    }
}