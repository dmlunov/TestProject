// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestGameHUD.generated.h"

class UMainMenu;
class UInteractionWidget;
struct FInteractableData;

UCLASS()
class TEST_API ATestGameHUD : public AHUD
{
	GENERATED_BODY()
public:

	virtual void DrawHUD() override;
              
    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UMainMenu> MainMenuClass;

    UPROPERTY(EditDefaultsOnly, Category = "Widgets")
    TSubclassOf<UInteractionWidget> InteractionWidgetClass;
    
    bool bIsMenuVisible;

    ATestGameHUD();

    void DisplayMenu();
    void HideMenu();

    void ShowInteractionWidget() const;
    void HideInteractionWidget() const;
    void UpdateInteractionWidget(const FInteractableData* InteractableData) const ;


protected:

    virtual void BeginPlay() override;

    UPROPERTY()
    UMainMenu* MainMenuWidget;

    UPROPERTY()
    UInteractionWidget* InteractionWidget;

private:
    void DrawCross();


};
