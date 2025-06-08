// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "MenuSystemGameInstance.h"
#include "LobbyGameState.h"     
#include "LobbyPlayerController.h" 
#include "MenuSystemCharacter.h"
#include "LobbyPlayerState.h"
#include "GameFramework/PlayerController.h"

//로비에서 사용하는 모드, 스테이트, 컨트롤러 바꿔주기
ALobbyGameMode::ALobbyGameMode()
{
    GameStateClass = ALobbyGameState::StaticClass();
    PlayerControllerClass = ALobbyPlayerController::StaticClass();
    PlayerStateClass = ALobbyPlayerState::StaticClass();

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

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    if (NextAvailableSlot > 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("최대 플레이어 수(4명)를 초과했습니다."));
        return;
    }

    if (ALobbyPlayerState* PS = Cast<ALobbyPlayerState>(NewPlayer->PlayerState))
    {
        PS->PlayerSlot = NextAvailableSlot++;
        PS->PlayerUID = PS->GetUniqueId().IsValid()
            ? PS->GetUniqueId()->ToString()
            : TEXT("UnknownID");

        UE_LOG(LogTemp, Log, TEXT("Player%d 접속: UID=%s"), PS->PlayerSlot, *PS->PlayerUID);
    }
}