// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectCoreTypes.h"
#include "ProjectGame/ProjectGame.h"
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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
    EPGAbilityInputID AbilityID{EPGAbilityInputID::None};

    // Tells an ability to activate immediately when its granted. Used for passive abilities and abilities forced on others.
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Ability")
    bool ActivateAbilityOnGranted = false;

    // If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
    // Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	
};
