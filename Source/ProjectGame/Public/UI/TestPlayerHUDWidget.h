// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectCoreTypes.h"
#include "TestPlayerHUDWidget.generated.h"


UCLASS()
class PROJECTGAME_API UTestPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIDate(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

private:
    /* UTestWeaponComponent* GetWeaponComponent() const;
    UHelthComponent* GetHealthComponent() const;*/
};
