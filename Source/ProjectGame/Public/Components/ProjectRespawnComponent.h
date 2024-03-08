// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectRespawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTGAME_API UProjectRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UProjectRespawnComponent();
    void Respawn(int32 RespawnTime);

    int32 GetRespawnCountDown() const { return RespawnCountDown; };
    bool IsRespawnInProgress() const;

protected:
    // Called when the game starts

private:
    // Called every frame

    FTimerHandle RespawnTimerHandle;
    int32 RespawnCountDown = 0;
    void RespawnTimerUpdate();
};
