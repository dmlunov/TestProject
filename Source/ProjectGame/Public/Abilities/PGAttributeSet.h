// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "PGAttributeSet.generated.h"

// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                       \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                             \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName);

UCLASS()
class PROJECTGAME_API UPGAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPGAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Health);

    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData MaxHealth;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, MaxHealth);

    // Health regen rate will passively increase Health every second
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
    FGameplayAttributeData HealthRegenRate;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, HealthRegenRate)

    // Current Mana, used to execute special abilities. Capped by MaxMana.
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
    FGameplayAttributeData Mana;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Mana)

    // MaxMana is its own attribute since GameplayEffects may modify it
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
    FGameplayAttributeData MaxMana;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, MaxMana)

    // Mana regen rate will passively increase Mana every second
    UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_ManaRegenRate)
    FGameplayAttributeData ManaRegenRate;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, ManaRegenRate)

    // Current stamina, used to execute special abilities. Capped by MaxStamina.
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Stamina)

    // MaxStamina is its own attribute since GameplayEffects may modify it
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, MaxStamina)

    // Stamina regen rate will passively increase Stamina every second
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
    FGameplayAttributeData StaminaRegenRate;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, StaminaRegenRate)

    // Damage — это метаатрибут, используемый DamageExecution для расчета окончательного урона, который затем превращается в -Health
    // Временное значение, существующее только на Сервере. Не тиражируется.
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Damage)

    UPROPERTY(BlueprintReadOnly, Category = "Character Level", ReplicatedUsing = OnRep_CharacterLevel)
    FGameplayAttributeData CharacterLevel;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, CharacterLevel)

        // Armor reduces the amount of damage done by attackers
     UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
    FGameplayAttributeData Armor;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Armor)

        // MoveSpeed affects how fast characters can move.
    UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
    FGameplayAttributeData MoveSpeed;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, MoveSpeed)



    // Experience points gained from killing enemies. Used to level up (not implemented in this project).
    UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XP)
    FGameplayAttributeData XP;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, XP)

    // Experience points awarded to the character's killers. Used to level up (not implemented in this project).
    UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XPBounty)
    FGameplayAttributeData XPBounty;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, XPBounty)

    // Gold gained from killing enemies. Used to purchase items (not implemented in this project).
    UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_Gold)
    FGameplayAttributeData Gold;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Gold)

    // Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
    UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_GoldBounty)
    FGameplayAttributeData GoldBounty;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, GoldBounty)

    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

    UFUNCTION()
    virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

    UFUNCTION()
    virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

    UFUNCTION()
    virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

    UFUNCTION()
    virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate);

    UFUNCTION()
    virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

    UFUNCTION()
    virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

    UFUNCTION()
    virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

    UFUNCTION()
    virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

    UFUNCTION()
    virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

    UFUNCTION()
    virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

    UFUNCTION()
    virtual void OnRep_XP(const FGameplayAttributeData& OldXP);

    UFUNCTION()
    virtual void OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty);

    UFUNCTION()
    virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);

    UFUNCTION()
    virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);

    /**
     * Вспомогательная функция для контрол значения атрибута в заданых дииапазонах
     */
    void AdjustAttributeForMaxChange(                     //
        const FGameplayAttributeData& AffectedAttribute,  //
        const FGameplayAttributeData& MaxAttribute,       //
        float NewMaxValue,                                //
        const FGameplayAttribute& AffectedAttributeProperty) const;

    private:
    FGameplayTag HitDirectionFrontTag;
    FGameplayTag HitDirectionBackTag;
    FGameplayTag HitDirectionRightTag;
    FGameplayTag HitDirectionLeftTag;

};
