// Test Game,  All Rights Reserved.

#include "TestGameModeBase.h"
#include "Player/ProjectBaseCharacter.h"
#include "Player/TestPlayerController.h"
#include "UI/TestGameHUD.h"
#include "Player/PGPlayerState.h"
#include "ProjectUtils.h"

#include "AIController.h"
// #include "Components/STURespawnComponent.h"
#include "Components/TestWeaponComponent.h"
#include "EngineUtils.h"  //получение всех акторов определеного типа

DEFINE_LOG_CATEGORY_STATIC(LogPGGameModeBase, All, All);

constexpr static int32 MinRaundTimeForRespawn = 10;

ATestGameModeBase::ATestGameModeBase()
{
    DefaultPawnClass = AProjectBaseCharacter::StaticClass();
    PlayerControllerClass = ATestPlayerController::StaticClass();
    HUDClass = ATestGameHUD::StaticClass();
    PlayerStateClass = APGPlayerState::StaticClass();
};

void ATestGameModeBase::StartPlay()
{
    Super::StartPlay();
    SpawnBot();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();

    // SetMatchState(ESTUMatchState::InProgress);
}

UClass* ATestGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ATestGameModeBase::SpawnBot()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        const auto PGAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

        RestartPlayer(PGAIController);  // функция которая создает павна RestartPlayer
    }
}

void ATestGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTime;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ATestGameModeBase::GameTimerUpdate, 1.0f, true);
}
void ATestGameModeBase::GameTimerUpdate()
{
    // UE_LOG(LogPGGameModeBase, Display, TEXT(" Round = %i/%i, Time = %i"), CurrentRound, GameData.RoundsNum, RoundCountDown);

    /* const auto TimerRate = GetWorldTimerManager().GetTimerRate(GameRoundTimerHandle);
     //GetTimerRate функция возвращает частоту срабатывания таймера
     RoundCountDown -= TimerRate;// пример как можно сделать с помощью GetTimerRate
     */

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            CurrentRound++;
            ResetPlayers();
            StartRound();
        }
        else
        {
            0;  //  GameOver();
        }
    }
}

void ATestGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        ResetOnePlayer(It->Get());
    }
}

void ATestGameModeBase::ResetOnePlayer(AController* Controller)
{
    if (Controller && Controller->GetPawn())
    {
        Controller->GetPawn()->Reset();
    }
    RestartPlayer(Controller);
    SetPlayerColor(Controller);
}

void ATestGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;
    int32 TeamID = 0;
    for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {

        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerState = Cast<APGPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        // установка имен игракам если человек то "Player" , AI="Bot"
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
       
        int32 AITeamID = Cast<AProjectBaseCharacter>(Controller->GetPawn())->TeamID;
        if (AITeamID != 0)
            TeamID = AITeamID;
        else
            TeamID = TeamID == 1 ? 2 : 1;

        PlayerState->SetTiamID(TeamID);
        PlayerState->SetTeamColor(DetermineColorByTeamID(TeamID)); 

        SetPlayerColor(Controller);
    }
}
FLinearColor ATestGameModeBase::DetermineColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }
    UE_LOG(LogPGGameModeBase, Display, TEXT("No Color for team id: %i, set to default: %s"), TeamID, *GameData.DefoultTeamColor.ToString());

    return GameData.DefoultTeamColor;
}
void ATestGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;
    const auto Character = Cast<AProjectBaseCharacter>(Controller->GetPawn());
    if (!Character) return;

    const auto PlayerState = Cast<APGPlayerState>(Controller->PlayerState);

    if (!PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

// void ATestGameModeBase::Killed(AController* KillerController, AController* ViktimController)
//{
//     const auto KillerPlayerState = KillerController ? Cast<APGPlayerState>(KillerController->PlayerState) : nullptr;
//     const auto ViktimPlayerState = ViktimController ? Cast<APGPlayerState>(ViktimController->PlayerState) : nullptr;
//
//     if (KillerPlayerState)
//     {
//         KillerPlayerState->AddKill();
//     }
//     if (ViktimPlayerState)
//     {
//         ViktimPlayerState->AddDeath();
//     }
//     StartRespawn(ViktimController);
// }
//
// void ATestGameModeBase::LogPlayerInfo()
//{
//     if (!GetWorld()) return;
//
//     for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
//     {
//         const auto Controller = It->Get();
//         if (!Controller) continue;
//         const auto PlayerState = Cast<APGPlayerState>(Controller->PlayerState);
//         if (!PlayerState) continue;
//
//         PlayerState->LogInfo();
//     }
// }
//
// void ATestGameModeBase::StartRespawn(AController* Controller)
//{
//     // const auto RespawnAvailable = RoundCountDown > MinRaundTimeForRespawn + GameData.RespawnTime;
//     // if (!RespawnAvailable) return;
//     // const auto RespawnComponent = Controller->GetComponentByClass<UPGRespawnComponent>();
//
//     // if (!RespawnComponent) return;
//     // RespawnComponent->Respawn(GameData.RespawnTime);
// }
//
// void ATestGameModeBase::RespawnRequest(AController* Controller)
//{
//     ResetOnePlayer(Controller);
// }
//
// void ATestGameModeBase::GameOver()
//{
//     UE_LOG(LogPGGameModeBase, Display, TEXT("=====Game Over======"));
//     LogPlayerInfo();
//     for (auto Pawn : TActorRange<APawn>(GetWorld()))  // собираем все павнов в игре !!!
//     {
//         if (Pawn)
//         {
//             Pawn->TurnOff();
//             Pawn->DisableInput(nullptr);
//         }
//     }
//     //
//     SetMatchState(ESTUMatchState::GameOver);
// }
////
// void ATestGameModeBase::SetMatchState(ESTUMatchState State)
//{
//     if (MatchState == State) return;
//     MatchState = State;
//     OnMatchStateChanged.Broadcast(MatchState);
// }
//
// bool ATestGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
//{
//     const auto PauseSet = Super::SetPause(PC, CanUnpauseDelegate);
//     if (PauseSet)
//     {
//         SetMatchState(ESTUMatchState::Pause);
//     }
//     return PauseSet;
//     // возвращаем значение родительской фунции PC, CanUnpauseDelegate это похоже глобальные переменные
// }
//
// bool ATestGameModeBase::ClearPause()
//{
//     const auto PauseCleared = Super::ClearPause();
//     if (PauseCleared)
//     {
//         StopAllFire();
//         SetMatchState(ESTUMatchState::InProgress);
//     }
//     return PauseCleared;
// }
//
// void ATestGameModeBase::StopAllFire()
//
//{
//     for (auto Pawn : TActorRange<APawn>(GetWorld()))
//     {
//         const auto WeaponComponent = Pawn->GetComponentByClass<UTestWeaponComponent>();
//         if (!WeaponComponent) continue;
//         WeaponComponent->StopFire();
//         // WeaponComponent->Zoom(false);
//     }
// }