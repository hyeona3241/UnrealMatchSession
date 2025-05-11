// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystemGameInstance.h"
#include "LobbyGameState.h"     
#include "LobbyPlayerController.h" 
#include "MenuSystemCharacter.h"

//�κ񿡼� ����ϴ� ���, ������Ʈ, ��Ʈ�ѷ� �ٲ��ֱ�
ALobbyGameMode::ALobbyGameMode()
{
    GameStateClass = ALobbyGameState::StaticClass();
    PlayerControllerClass = ALobbyPlayerController::StaticClass();

    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter.BP_ThirdPersonCharacter_C")
    );
    if (PlayerPawnBPClass.Succeeded())
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void ALobbyGameMode::BeginPlay()
{
    Super::BeginPlay();

    // GameInstance���� ������ �� �� �̸� ��������
    if (UMenuSystemGameInstance* GI = Cast<UMenuSystemGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        const FString DesiredMap = GI->DesiredMap;

        // GameState�� ����
        if (ALobbyGameState* GS = GetGameState<ALobbyGameState>())
        {
            GS->SelectedMap = DesiredMap;
            UE_LOG(LogTemp, Log, TEXT("[LobbyGameMode] SelectedMap replicated to GameState: %s"), *DesiredMap);
        }
    }
}

