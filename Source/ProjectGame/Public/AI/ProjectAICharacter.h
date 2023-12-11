// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ProjectBaseCharacter.h"
#include "ProjectAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTGAME_API AProjectAICharacter : public AProjectBaseCharacter
{
	GENERATED_BODY()
	
	AProjectAICharacter(const FObjectInitializer& ObjInit);

	protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

};
