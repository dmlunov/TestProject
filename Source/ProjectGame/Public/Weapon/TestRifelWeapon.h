// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TestBaseWeapon.h"
#include "TestRifelWeapon.generated.h"

class UPGWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;

/**
 *
 */
UCLASS()
class PROJECTGAME_API ATestRifelWeapon : public ATestBaseWeapon
{
    GENERATED_BODY()

public:
    ATestRifelWeapon();
    virtual void StartFire() override;
    virtual void StopFire() override;


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float BulletSpread = 1.5f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TimerBetwenShots = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount = 10.0f;

     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FString TraceTargetName = "TraceTarget";

     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UPGWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;
    virtual void MakeShot() override;

    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

    void MakeDamage(const FHitResult& HitResult);



private:
    FTimerHandle ShotTimerHandle;

   

    UPROPERTY()
    UAudioComponent* FireAudioComponent;

    UPROPERTY()
    UNiagaraComponent* MuzzleFXComponent;

    void InitMuzzleFX();
    void SetMuzzleFXVisibility(bool Visibile);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
