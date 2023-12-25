// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PGFindEnemyService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGFindEnemyService : public UBTService
{
	GENERATED_BODY()
public:
    UPGFindEnemyService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector EnemyActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
