// Test Game,  All Rights Reserved.


#include "Menu/PGMenuGameModeBase.h"
#include "Menu/PGMenuPlayerController.h"
#include "Menu/UI/PGMenuHUD.h"
//#include "PGGameInstance.h"

APGMenuGameModeBase::APGMenuGameModeBase()
{
    PlayerControllerClass = APGMenuPlayerController::StaticClass();
    HUDClass = APGMenuHUD::StaticClass();
}
