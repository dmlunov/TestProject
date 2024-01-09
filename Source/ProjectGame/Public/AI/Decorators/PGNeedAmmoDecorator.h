// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PGNeedAmmoDecorator.generated.h"

class  ATestBaseWeapon;
/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGNeedAmmoDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
    UPGNeedAmmoDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<ATestBaseWeapon> WeaponType;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};