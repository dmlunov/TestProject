// Test Game,  All Rights Reserved.

#include "UI/TestGameHUD.h"
#include "Engine/Canvas.h"
#include "UI/MainMenu.h"
#include "UI/Interaction/InteractionWidget.h"
#include "Interface/TestBaseInterface.h"
#include "Blueprint/UserWidget.h"
#include "UI/Inventory/InventoryActionMessage.h"
#include "UI/TestPlayerHUDWidget.h"
#include "UI/PGStartLoginMenu.h"
#include "TestGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(TestGameHUDLog, All, All);

ATestGameHUD::ATestGameHUD()
{
    //
}

void ATestGameHUD::BeginPlay()
{
    Super::BeginPlay();

    //if (PGStartLoginMenuClass)
    //{
    //    PGStartLoginMenuWidget = CreateWidget<UPGStartLoginMenu>(GetWorld(), PGStartLoginMenuClass);
    //    PGStartLoginMenuWidget->AddToViewport();
    //    PGStartLoginMenuWidget->SetVisibility(ESlateVisibility::Visible);

    //}

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
    //// удалить
    // if (PlayerHUDWidgetClass)
    //{
    //     PlayerHUDWidget = CreateWidget<UTestPlayerHUDWidget>(GetWorld(), PlayerHUDWidgetClass);
    //     PlayerHUDWidget->AddToViewport();
    //     PlayerHUDWidget->SetVisibility(ESlateVisibility::Collapsed);
    // }
    if (PlayerHUDWidgetClass)
    {
        GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    }
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
        if (!GameWidget) continue;
        GameWidget->AddToViewport();
        GameWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GetWorld())
    {
        const auto GameMode = Cast<ATestGameModeBase>(GetWorld()->GetAuthGameMode());
        if (GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ATestGameHUD::OnMatchStateChanged);
        }
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


void ATestGameHUD::OnMatchStateChanged(ESTUMatchState State)

{
    UE_LOG(TestGameHUDLog, Display, TEXT("Match state change : % s") , *UEnum::GetValueAsString(State) );

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(State))  // GameWidgets.Contains(State) проверка существует ли данный элемент с указаным ключем
    {
        CurrentWidget = GameWidgets[State];
    }

    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
       // CurrentWidget->Show();
    }
}