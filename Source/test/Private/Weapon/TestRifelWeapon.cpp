// Test Game,  All Rights Reserved.

#include "Weapon/TestRifelWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Components/HelthComponent.h"
#include "Player/TestBaseCharacter.h"

void ATestRifelWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATestRifelWeapon::MakeShot, TimerBetwenShots, true);
}

void ATestRifelWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ATestRifelWeapon::MakeShot()
{
    if (!GetWorld()) return;

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        MakeDamage(HitResult);
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::White, false, 0.1f, 0, 0.5f);

        AActor* HitActor = HitResult.GetActor();
        if (HitActor && HitActor->FindComponentByClass<UHelthComponent>())
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.0f, 12, FColor::Red, false, 0.5f);
        else
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.0f, 12, FColor::White, false, 0.5f);

        // UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString())
        //  TakeDamage(0.1f, FDamageEvent{}, Controller, this);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::White, false, 0.1f, 0, 0.5f);
    }
}

bool ATestRifelWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);  // SocketTransform.GetRotation().GetForwardVector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ATestRifelWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;
    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}