// Test Game,  All Rights Reserved.

#include "TestGameModeBase.h"
#include "Player/ProjectBaseCharacter.h"
#include "Player/TestPlayerController.h"
#include "UI/TestGameHUD.h"

 ATestGameModeBase::ATestGameModeBase()
{
    DefaultPawnClass = AProjectBaseCharacter::StaticClass();
    PlayerControllerClass = ATestPlayerController::StaticClass();
    HUDClass = ATestGameHUD::StaticClass();
};