// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectCoreTypes.h"
#include "PGMenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class UPGGameInstance;
class UPGLevelItemWidget;
class USoundCue;

UCLASS()
class PROJECTGAME_API UPGMenuWidget : public UUserWidget
{

    GENERATED_BODY()
protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

//    UPROPERTY(meta = (BindWidget))
//    UHorizontalBox* LevelItemsBox;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
//    TSubclassOf<UUserWidget> LevelItemWidgetClass;
//
//    UPROPERTY(meta = (BindWidgetAnim), Transient)
//    UWidgetAnimation* HideAnimation;
//
//    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
//    USoundCue* StaptGameSound;
//
//    virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
//
//private:
//    UPROPERTY()
//    TArray<UPGLevelItemWidget*> LevelItemWidgets;
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnQuitGame();
//
//    void InitLevelItems();  // для создания кнопок
//
//    void OnLevelSelected(const FLevelData& Data);  // для делегата , при нажатии на одну из кнопок
//
    UPGGameInstance* GetPGGameInstans() const;
};
