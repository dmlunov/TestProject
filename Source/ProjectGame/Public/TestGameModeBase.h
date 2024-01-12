// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectCoreTypes.h"
#include "TestGameModeBase.generated.h"


class AAIController;


UCLASS()
class PROJECTGAME_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
    ATestGameModeBase();

    virtual void StartPlay() override;

   // virtual APawn* SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot) override;
    
   
   virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

    //void Killed(AController* KillerController, AController* ViktimController);

    //FGameData GetGameData() const { return GameData; };
    //int32 GetCurrentRoundNum() { return CurrentRound; };
    //int32 GetRoundSecondRemaining() { return RoundCountDown; };

    //void RespawnRequest(AController* Controller);
    ////
    //FOnMatchStateChangedSignature OnMatchStateChanged;

    //virtual bool SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate = FCanUnpause()) override;

    //virtual bool ClearPause() override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<APawn> AIPawnClass;


    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

    // virtual void BeginPlay() override;

private:
    //
    ESTUMatchState MatchState = ESTUMatchState::WaitingToStart;

    int32 CurrentRound = 1;
    int32 RoundCountDown = 0;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBot();
    void StartRound();
    void GameTimerUpdate();

    void ResetPlayers();
    void ResetOnePlayer(AController* Controller);

    void CreateTeamsInfo();
    FLinearColor DetermineColorByTeamID(int32 TeamID) const;

    void SetPlayerColor(AController* Controller);


    //void LogPlayerInfo();

    //void StartRespawn(AController* Controller);

    //void GameOver();
    ////
    //void SetMatchState(ESTUMatchState);

    //void StopAllFire();
};
