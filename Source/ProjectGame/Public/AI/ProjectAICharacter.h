// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ProjectBaseCharacter.h"
#include "ProjectAICharacter.generated.h"

class UBehaviorTree;  
class UWidgetComponent;
/**
 *
 */
UCLASS()
class PROJECTGAME_API AProjectAICharacter : public AProjectBaseCharacter
{
    GENERATED_BODY()

public:
    AProjectAICharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    UBehaviorTree* BehaviorTreeAsset;

    virtual void Tick(float DeltaTime) override;



protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UWidgetComponent* HealthWidgetComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
    float HealthVisibilityDistance = 1000.0f;

    virtual void BeginPlay() override;

    virtual void OnDeath() override;

   // virtual void OnHealthChanged(float Health) override;

private:
    void UpdateHealthWidgetVisibility();
};
