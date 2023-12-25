// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PGAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API UPGAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
public:
    AActor* GetClosesEnemy() const;
};
