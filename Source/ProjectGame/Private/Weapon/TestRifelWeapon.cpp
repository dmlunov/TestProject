// Test Game,  All Rights Reserved.

#include "Weapon/TestRifelWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Components/HelthComponent.h"
#include "Player/ProjectBaseCharacter.h"
#include "Weapon/Components/PGWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(RifelWeaponLog, All, All)

ATestRifelWeapon::ATestRifelWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<UPGWeaponFXComponent>("WeaponFXComponent");
}
void ATestRifelWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponFXComponent);
}

void ATestRifelWeapon::StartFire()
{
    InitMuzzleFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ATestRifelWeapon::MakeShot, TimerBetwenShots, true);
    MakeShot();
}

void ATestRifelWeapon::StopFire()
{

    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetMuzzleFXVisibility( false);
}

void ATestRifelWeapon::MakeShot()
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
    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);
    
    FVector TraceFXEnd = TraceEnd;

    if (HitResult.bBlockingHit)
    {
        TraceFXEnd = HitResult.ImpactPoint;

        AActor* HitActor = HitResult.GetActor();

        if (HitActor && HitActor->FindComponentByClass<UHelthComponent>())
        {

            UE_LOG(RifelWeaponLog, Display, TEXT("Bone: %s"), *HitResult.PhysMaterial.Get()->GetName())
            // * HitResult.BoneName.ToString())
            MakeDamage(HitResult);
            // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 0.1f, 0, 0.5f);
            // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.0f, 12, FColor::Red, false, 0.5f);
        }
       // else
       // {
           // DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::White, false, 0.1f, 0, 0.5f);
           // DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 2.0f, 12, FColor::White, false, 0.5f);
       // }
        WeaponFXComponent->PlayImpactFX(HitResult);
        SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
        // UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString())
        //  TakeDamage(0.1f, FDamageEvent{}, Controller, this);
    }
    else
    {
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::White, false, 0.1f, 0, 0.5f);
    }
    DecreaseAmmo();
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

void ATestRifelWeapon::InitMuzzleFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    if (!FireAudioComponent)
    {
       // FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }

    SetMuzzleFXVisibility(true);
}
void ATestRifelWeapon::SetMuzzleFXVisibility(bool Visibile)
{
    UE_LOG(RifelWeaponLog, Display, TEXT("SetMuzzleFXVisibility"));
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!Visibile);
        MuzzleFXComponent->SetVisibility(Visibile, true);

    }

    if (FireAudioComponent)
    {
       // FireAudioComponent->SetPaused(!Visibile);
    }
}

void ATestRifelWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}