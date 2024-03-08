// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectCoreTypes.h"
#include "GameOverWidget.generated.h"


class UVerticalBox;  
class UButton;


UCLASS()
class PROJECTGAME_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // virtual bool Initialize() override;//удалили потомучно используем NativeOnInitialized()

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PlayerStateBox;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatRowWidgetClass;

    UPROPERTY(meta = (BindWidget))
    UButton* ResetLevelButton;

    virtual void NativeOnInitialized() override;  // фукция вызывается если инициализация Initialize() прошла успешно

private:
    void OnMatchStateChanged(ESTUMatchState State);
    void UpdatePlayerStat();

    UFUNCTION()
    void OnResetLevel();
};
