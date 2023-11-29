// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TestBaseWeapon.h"
#include "TestShotgunWeapon.generated.h"

class ATestProjecttile;

UCLASS()
class PROJECTGAME_API ATestShotgunWeapon : public ATestBaseWeapon
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



    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ATestProjecttile> ProjecttileClass;

private:
    FTimerHandle ShotTimerHandle;
};
