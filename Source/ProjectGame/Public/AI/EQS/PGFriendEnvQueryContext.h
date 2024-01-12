// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "PGFriendEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGFriendEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
    virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

protected:
    UPROPERTY(EditAnywhere, Category = "AI")
    FName EnemyActorKeyName = "FriendActor";
};