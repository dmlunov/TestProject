// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PGAIController.generated.h"

class UPGAIPerceptionComponent;
class UPGRespawnComponent;
/**
 * 
 */
UCLASS()
class PROJECTGAME_API APGAIController : public AAIController
{
	GENERATED_BODY()
public:
    APGAIController();


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UPGAIPerceptionComponent* PGAIPerceptionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FName FocusOnKeyName = "EnemyActor";  // имя ключа из BehaviorTree в котором содержиться указатель на врага

    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaTime) override;

   // UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
   // USTURespawnComponent* RespawnComponent;

private:
    AActor* GetFocusOnActor() const;
};
