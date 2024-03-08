// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGPauseWidget.generated.h"

class UButton;


UCLASS()
class PROJECTGAME_API UPGPauseWidget : public UUserWidget
{
	GENERATED_BODY()
public:

protected:
    // создаем кнопку , и добпаляем мета информацию которая асоциирует кнопку в блюпринте с кнопкой которую будем использовать в с++
    UPROPERTY(meta = (BindWidget))
    UButton* ClearPauseButton;  // тако же имя должно быть у кнопки в виджате
    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()  // добали UFUNCTION() потому что подписываемся к динамическому делегату
    void OneClearPause();
};
