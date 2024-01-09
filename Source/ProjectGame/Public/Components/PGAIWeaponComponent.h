// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/TestWeaponComponent.h"
#include "PGAIWeaponComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGAIWeaponComponent : public UTestWeaponComponent
{
	GENERATED_BODY()
public:
    virtual void StartFire() override;
    virtual void NextWeapon() override;

protected:
    virtual void BeginPlay() override;
};
