// Test Game,  All Rights Reserved.


#include "UI/PGStartLoginMenu.h"
#include "UI/TestGameHUD.h"
#include "UI/TestPlayerHUDWidget.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

#include "Kismet/GamePlayStatics.h"

//#include "Kismet/KismetSystemLibrary.h"

void UPGStartLoginMenu::NativeConstruct()
{
    Super::NativeConstruct();

     if (LoginButton)
    {
         LoginButton->OnClicked.AddDynamic(this, &UPGStartLoginMenu::ActionLogin);
    }
    if (QuitButton)
    {
         QuitButton->OnClicked.AddDynamic(this, &UPGStartLoginMenu::ActionQuit);
    }
}

void UPGStartLoginMenu::ActionLogin()
{
    //

    auto TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (!TestGameHUD) return;
    //auto PlayerHUDWidget = TestGameHUD->GetPlayerHUDWidget();
    //PlayerHUDWidget->SetVisibility(ESlateVisibility::Visible);
    //this->SetVisibility(ESlateVisibility::Collapsed);

    FName StartupLevelName = "ProjectGameLevel_00";
    UGameplayStatics::OpenLevel(this, StartupLevelName);


    //UGamePlayStatics
 }

void UPGStartLoginMenu::ActionQuit()
{
    //FGenericPlatformMisc::RequestExit(false);
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);

 }

