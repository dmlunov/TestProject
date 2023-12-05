// Test Game,  All Rights Reserved.

#include "UI/TestGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/MainMenu.h"
#include "UI/Interaction/InteractionWidget.h"
#include "Interface/TestBaseInterface.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryActionMessage.h"

ATestGameHUD::ATestGameHUD()
{
    //
}

void ATestGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MainMenuClass)
    {
        MainMenuWidget = CreateWidget<UMainMenu>(GetWorld(), MainMenuClass);  // UMainMenu::StaticClass());
        MainMenuWidget->AddToViewport(5);
        MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (InteractionWidgetClass)
    {
        InteractionWidget = CreateWidget<UInteractionWidget>(GetWorld(), InteractionWidgetClass);
        InteractionWidget->AddToViewport(-1);
        InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    }

    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void ATestGameHUD::DrawHUD()
{
    Super::DrawHUD();

   // DrawCross();
}

void ATestGameHUD::DrawCross()
{
    int32 SizeX = Canvas->SizeX;
    int32 SizeY = Canvas->SizeY;
    const TInterval<float> Center(SizeX * 0.5f, SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

void ATestGameHUD::DisplayMenu()
{
    if (MainMenuWidget)
    {
        bIsMenuVisible = true;
        MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ATestGameHUD::HideMenu() 
{
    if (MainMenuWidget)
    {
        bIsMenuVisible = false;
        MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
        MainMenuWidget->InventoryActionMessage->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ATestGameHUD::ShowInteractionWidget() const
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void ATestGameHUD::HideInteractionWidget() const
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void ATestGameHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
    if (InteractionWidget)
    {
        if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
        {
            InteractionWidget->SetVisibility(ESlateVisibility::Visible);
        }

        InteractionWidget->UpdateWidget(InteractableData);
        

    }
}

 void ATestGameHUD::ToggleMenu() 
 {
     if (bIsMenuVisible)
    {
        HideMenu();
        const FInputModeGameOnly InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(false);
     }
    else
    {
        DisplayMenu();
        const FInputModeGameAndUI InputMode;
        GetOwningPlayerController()->SetInputMode(InputMode);
        GetOwningPlayerController()->SetShowMouseCursor(true);
     }
 }