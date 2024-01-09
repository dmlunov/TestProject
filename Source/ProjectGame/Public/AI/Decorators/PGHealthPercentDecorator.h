// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "PGHealthPercentDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGHealthPercentDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
    UPGHealthPercentDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float HealthPercent = 0.6f;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
