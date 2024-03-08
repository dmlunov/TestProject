// Test Game,  All Rights Reserved.

#include "Menu/UI/PGMenuHUD.h"
#include "Blueprint/UserWidget.h"
// #include "UI/PGBaseWidget.h"

void APGMenuHUD::BeginPlay()
{
    Super::BeginPlay();
    if (MenuWidgetClass)
    {

        const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);

        if (MenuWidget)
        {
            MenuWidget->AddToViewport();
           // MenuWidget->Show();
        }
    }
}
