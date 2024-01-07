// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PGChangeWeaponService.generated.h"

/**
 *
 */
UCLASS()
class PROJECTGAME_API UPGChangeWeaponService : public UBTService
{
    GENERATED_BODY()

public:

    UPGChangeWeaponService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Probability = 0.5f;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};