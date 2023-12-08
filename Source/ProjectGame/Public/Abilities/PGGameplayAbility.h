// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectCoreTypes.h"
#include "PGGameplayAbility.generated.h"




UCLASS()
class PROJECTGAME_API UPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	UPGGameplayAbility();

	// Способности активируются при нажатии ввода
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
    EPGAbilityInputID AbilityInputID{EPGAbilityInputID::None};
	
};
