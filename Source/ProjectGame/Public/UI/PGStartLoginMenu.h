// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGStartLoginMenu.generated.h"

class UBorder;
class UButton;
class UTextBlock;


UCLASS()
class PROJECTGAME_API UPGStartLoginMenu : public UUserWidget
{
    GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;

    UPROPERTY(VisibleAnywhere, Category = "Menu Button", meta = (BindWidget))
    UButton* LoginButton;

    UPROPERTY(VisibleAnywhere, Category = "Menu Button", meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* LoginText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuitText;



    UFUNCTION()
    void ActionLogin();

    UFUNCTION()
    void ActionQuit();

};
