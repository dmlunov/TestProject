// Test Game,  All Rights Reserved.


#include "Abilities/PGAttributeSet.h"
#include "Net/UnrealNetwork.h"


UPGAttributeSet::UPGAttributeSet(){
    //
};

void UPGAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UPGAttributeSet, Health);
};


void UPGAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)

{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UPGAttributeSet, Health, OldValue);

 
}