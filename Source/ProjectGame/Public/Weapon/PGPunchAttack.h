// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TestBaseWeapon.h"
#include "PGPunchAttack.generated.h"

class USphereComponent;
/**
 *
 */
UCLASS()
class PROJECTGAME_API APGPunchAttack : public ATestBaseWeapon
{
    GENERATED_BODY()

public:

    APGPunchAttack();

    UFUNCTION()
    virtual void StartFire() override;

protected:

    UFUNCTION()
    void MakePunch
    (UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult);
    //(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,  const FHitResult& Hit);

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
    USphereComponent* HitCollision;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* PunchAnimMontage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    AActor* HitActor;

    UFUNCTION()
    void MakeDamage(const FHitResult& HitResult);
};
