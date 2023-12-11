// Test Game,  All Rights Reserved.


#include "Player/TestPlayerController.h"
#include "UI/TestGameHUD.h"

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();
    TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}


ATestGameHUD* ATestPlayerController::GetHUD()
{
    return TestGameHUD;
 }