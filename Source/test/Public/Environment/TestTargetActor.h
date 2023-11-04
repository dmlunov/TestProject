// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestTargetActor.generated.h"

class UHelthComponent;
class UTextRenderComponent;

UCLASS()
class TEST_API ATestTargetActor : public AActor
{
    GENERATED_BODY()

public:
    ATestTargetActor();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* StaticMeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UHelthComponent* HelthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

public:
private:
    void OnDeath();
    void OnHealthChanged(float Health);
    float MaxHealth;
};
