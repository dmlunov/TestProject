// Test Game,  All Rights Reserved.


#include "Weapon/TestShotgunWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Components/HelthComponent.h"
#include "Player/ProjectBaseCharacter.h"
#include "Weapon/TestProjecttile.h"

void ATestShotgunWeapon::StartFire()
{

    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATestShotgunWeapon::MakeShot, TimerBetwenShots, true);
    MakeShot();
}

void ATestShotgunWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

void ATestShotgunWeapon::MakeShot()
{

    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }
    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    // auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjecttileClass, SpawnTransform);

    ATestProjecttile* Projecttile = GetWorld()->SpawnActorDeferred<ATestProjecttile>(ProjecttileClass, SpawnTransform);

    if (Projecttile)
    {
        Projecttile->SetShotDirection(Direction);
        Projecttile->SetOwner(GetOwner());
        Projecttile->FinishSpawning(SpawnTransform);
    }

    // UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
    DecreaseAmmo();
}

bool ATestShotgunWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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

