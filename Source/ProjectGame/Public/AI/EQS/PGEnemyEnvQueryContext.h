// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "PGEnemyEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

	protected:
    UPROPERTY(EditAnywhere, Category = "AI")
        FName EnemyActorKeyName = "EnemyActor";
};
