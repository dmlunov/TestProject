// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TestBaseWeapon.h"
#include "TestRifelWeapon.generated.h"

/**
 *
 */
UCLASS()
class PROJECTGAME_API ATestRifelWeapon : public ATestBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimerBetwenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

    virtual void MakeShot() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    void MakeDamage(const FHitResult& HitResult);



private:
    FTimerHandle ShotTimerHandle;
};
