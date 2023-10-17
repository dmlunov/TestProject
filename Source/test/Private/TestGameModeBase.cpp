// Test Game,  All Rights Reserved.

#include "TestGameModeBase.h"
#include "Player/TestBaseCharacter.h"
#include "Player/TestPlayerController.h"

ATestGameModeBase::ATestGameModeBase(){
    DefaultPawnClass = ATestBaseCharacter::StaticClass();
    PlayerControllerClass = ATestPlayerController::StaticClass();
};