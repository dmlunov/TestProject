// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "PGFindFriendService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGFindFriendService : public UBTService
{
	GENERATED_BODY()
public:
    UPGFindFriendService();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FBlackboardKeySelector FriendActorKey;

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};