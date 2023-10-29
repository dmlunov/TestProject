// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestProjecttile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class TEST_API ATestProjecttile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATestProjecttile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRadius = 200.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float LifeProgectile = 5.0f;

public:
    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; };

private:
    FVector ShotDirection;

     // FComponentHitSignature
    UFUNCTION()
    void OnProjecttileHit (
        UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    AController* GetController() const;
};
