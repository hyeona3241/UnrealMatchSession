// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystemGameInstance.h"
#include "LobbyGameState.h"     
#include "LobbyPlayerController.h" 
#include "MenuSystemCharacter.h"

//로비에서 사용하는 모드, 스테이트, 컨트롤러 바꿔주기
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

    // GameInstance에서 저장해 둔 맵 이름 가져오기
    if (UMenuSystemGameInstance* GI = Cast<UMenuSystemGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        const FString DesiredMap = GI->DesiredMap;

        // GameState에 복사
        if (ALobbyGameState* GS = GetGameState<ALobbyGameState>())
        {
            GS->SelectedMap = DesiredMap;
            UE_LOG(LogTemp, Log, TEXT("[LobbyGameMode] SelectedMap replicated to GameState: %s"), *DesiredMap);
        }
    }
}

