// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestGameHUD.generated.h"

class UMainMenu;
class UInteractionWidget;
class UTestPlayerHUDWidget;
struct FInteractableData;

UCLASS()
class PROJECTGAME_API ATestGameHUD : public AHUD
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
    void ToggleMenu();

    void ShowInteractionWidget() const;
    void HideInteractionWidget() const;
    void UpdateInteractionWidget ( const FInteractableData* InteractableData) const ;

    UFUNCTION()
    FORCEINLINE UMainMenu* GetMainMenuWidget() const { return MainMenuWidget;};
    UFUNCTION()
    FORCEINLINE UTestPlayerHUDWidget* GetPlayerHUDWidget() const { return PlayerHUDWidget; };

protected:

    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UTestPlayerHUDWidget> PlayerHUDWidgetClass;

    UPROPERTY()
    UMainMenu* MainMenuWidget;

    UPROPERTY()
    UInteractionWidget* InteractionWidget;

    UPROPERTY()
    UTestPlayerHUDWidget* PlayerHUDWidget;

private:
    void DrawCross();


};
