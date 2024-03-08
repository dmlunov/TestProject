// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectCoreTypes.h"
#include "TestPlayerHUDWidget.generated.h"

class AProjectBaseCharacter;
class UTextBlock;

UCLASS()
class PROJECTGAME_API UTestPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetManaPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetStaminaPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetHealth(float Health);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetMana(float Mana);
    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetStamina(float Stamina);

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIDate(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerAlive() const;
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool IsPlayerSpectating() const;

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void ShowAbilityConfirmCancelText(bool ShowText);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetRespawnCountdown(float RespawnTimeRemaining);



    /**
     * Attribute setters
     */
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetMaxHealth(float MaxHealth);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCurrentHealth(float CurrentHealth);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthPercentage(float HealthPercentage);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHealthRegenRate(float HealthRegenRate);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetMaxStamina(float MaxStamina);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCurrentStamina(float CurrentStamina);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetStaminaPercentage(float StaminaPercentage);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetStaminaRegenRate(float StaminaRegenRate);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetMaxMana(float MaxMana);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetCurrentMana(float CurrentMana);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetManaPercentage(float ManaPercentage);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetManaRegenRate(float ManaRegenRate);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetExperience(int32 Experience);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetHeroLevel(int32 HeroLevel);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void SetGold(int32 Gold);

protected:
    virtual void NativeConstruct() override;
    AProjectBaseCharacter* Character;

     virtual void NativeOnInitialized() override;


    UPROPERTY(VisibleAnywhere, Category = "Attribute", meta = (BindWidget))
    UTextBlock* HealthText;
    UPROPERTY(VisibleAnywhere, Category = "Attribute", meta = (BindWidget))
    UTextBlock* ManaText;
    UPROPERTY(VisibleAnywhere, Category = "Attribute", meta = (BindWidget))
    UTextBlock* StaminaText;

    /*UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* DamageAnimation;*/

    UFUNCTION(BlueprintImplementableEvent,
    Category = "UI")  // спецификатор BlueprintImplementableEvent позволит создать event в bliprint на основе этой функции
    void OnTakeDamage();

private:
    /* UTestWeaponComponent* GetWeaponComponent() const;
    UHelthComponent* GetHealthComponent() const;*/

     //void UpdateHealthBar();

    void OnHealthChanged(float Health, float HealthDelta);  //, float HealthDelta); 
    void OnNewPawn(APawn* NewPawn);

};
