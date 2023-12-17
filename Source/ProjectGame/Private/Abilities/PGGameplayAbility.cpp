// Test Game,  All Rights Reserved.


#include "Abilities/PGGameplayAbility.h"
#include "Components/PGAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(PGGameplayAbilityLog, All, All);
UPGGameplayAbility::UPGGameplayAbility()
{
    // Default to Instance Per Actor
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    // Default tags that block this ability from activating
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void UPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);

    UE_LOG(PGGameplayAbilityLog, Display, TEXT("PGGameplayAbility started "));

    if (ActivateAbilityOnGranted)
    {
        ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
       
    }
}