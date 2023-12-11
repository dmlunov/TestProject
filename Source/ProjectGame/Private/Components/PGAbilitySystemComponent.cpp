// Test Game,  All Rights Reserved.


#include "Components/PGAbilitySystemComponent.h"


void UPGAbilitySystemComponent::ReceiveDamage(UPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
    ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}