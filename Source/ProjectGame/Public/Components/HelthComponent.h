// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectCoreTypes.h"
#include "HelthComponent.generated.h"

class UPGAttributeSet;
class AProjectBaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent)) class PROJECTGAME_API UHelthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHelthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable)
    virtual bool IsDead() const { return FMath::IsNearlyZero(Health) || Health < 0; }

    UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / MaxHealth; };

    float GetHealth() const { return Health; };
    float GetMaxHealth() const { return MaxHealth; };
    void SetAutoHeal(bool AHeal) { AutoHeal = AHeal; };

    UFUNCTION()
    virtual void SetHealth(float NewHealth);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

    virtual void BeginPlay() override;

    UPROPERTY()
    AProjectBaseCharacter* Character;

private:
    float Health = 0.0f;

    FTimerHandle HealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void HealUpdate();

   // UPROPERTY()
   // UPGAttributeSet* Attributes;
};
