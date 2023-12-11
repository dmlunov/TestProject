// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "PGAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(
    FReceivedDamageDelegate,
    UPGAbilitySystemComponent*,SourceASC, 
    float,UnmitigatedDamage,
    float, MitigatedDamage);


UCLASS()
class PROJECTGAME_API UPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	bool bCharacterAbilitiesGiven = false;
    bool bStartupEffectsApplied = false;

    FReceivedDamageDelegate ReceivedDamage;


    // Вызывается из GDDamageExecCalculation. Вызов сообщения о получении урона когда этот ASC получает урон.
    virtual void ReceiveDamage(UPGAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);
};
