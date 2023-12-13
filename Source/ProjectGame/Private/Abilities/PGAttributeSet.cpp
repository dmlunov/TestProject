// Test Game,  All Rights Reserved.

#include "Abilities/PGAttributeSet.h"
#include "Net/UnrealNetwork.h"
// #include "GameplayEffectTypes.h"
// #include "AbilitySystemComponent.h"
#include "GameplayEffectExtension.h"
#include "Player/ProjectBaseCharacter.h"
// #include "Components/HelthComponent.h"
// #include "ProjectUtils.h"

DEFINE_LOG_CATEGORY_STATIC(PGAttributeSetLog, All, All)

UPGAttributeSet::UPGAttributeSet(){};

void UPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UPGAttributeSet, Health);

    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, Mana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, ManaRegenRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, CharacterLevel, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, Armor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);

    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, XP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, XPBounty, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, Gold, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UPGAttributeSet, GoldBounty, COND_None, REPNOTIFY_Always);
};

void UPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

    float DeltaValue{0.0f};

    if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Type::Additive)
    {
        // If this was additive, store the raw delta value to be passed along later
        DeltaValue = Data.EvaluatedData.Magnitude;
    }

    AProjectBaseCharacter* TargetCharacter{nullptr};

    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        // AController* TargetController = nullptr;
        AActor* TargetActor{nullptr};
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        // TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<AProjectBaseCharacter>(TargetActor);
    }

    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {

        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
        //UE_LOG(PGAttributeSetLog, Display, TEXT("Health is = %f"), GetHealth());
        // if (TargetCharacter)  TargetCharacter->HandleHealthChanged(DeltaValue, SourceTags);
    }
}

void UPGAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{

    Super::PreAttributeChange(Attribute, NewValue);

    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
}

void UPGAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& AffectedAttribute,  //
    const FGameplayAttributeData& MaxAttribute,                                                     //
    float NewMaxValue,                                                                              //
    const FGameplayAttribute& AffectedAttributeProperty) const
{
    //
    UAbilitySystemComponent* AbilityComponent = GetOwningAbilitySystemComponent();
    const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
    if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComponent)
    {
        // Change current value to maintain the current Val / Max percent
        const float CurrentValue = AffectedAttribute.GetCurrentValue();
        const float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

        AbilityComponent->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }
}

void UPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Health, OldValue);
}

void UPGAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, MaxHealth, OldValue);
}

void UPGAttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, HealthRegenRate, OldHealthRegenRate);
}

void UPGAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Mana, OldMana);
}

void UPGAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, MaxMana, OldMaxMana);
}

void UPGAttributeSet::OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, ManaRegenRate, OldManaRegenRate);
}

void UPGAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Stamina, OldStamina);
}

void UPGAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, MaxStamina, OldMaxStamina);
}

void UPGAttributeSet::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, StaminaRegenRate, OldStaminaRegenRate);
}

void UPGAttributeSet::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, CharacterLevel, OldCharacterLevel);
}

void UPGAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Armor, OldArmor);
}

void UPGAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, MoveSpeed, OldMoveSpeed);
}

void UPGAttributeSet::OnRep_XP(const FGameplayAttributeData& OldXP)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, XP, OldXP);
}

void UPGAttributeSet::OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, XPBounty, OldXPBounty);
}

void UPGAttributeSet::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Gold, OldGold);
}

void UPGAttributeSet::OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, GoldBounty, OldGoldBounty);
}
