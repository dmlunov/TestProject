// Test Game,  All Rights Reserved.


#include "Weapon/TestLauncherWeapon.h"
#include "Weapon/TestProjecttile.h"
//#include "Kismet/GameplayStatics.h"

void ATestLauncherWeapon::StartFire()
{
    MakeShot();
 }


void ATestLauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsAmmoEmpty()) return;

    FVector TraceStart;
    FVector TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();


    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    //auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjecttileClass, SpawnTransform);

    ATestProjecttile* Projecttile = GetWorld()->SpawnActorDeferred<ATestProjecttile>(ProjecttileClass, SpawnTransform);

    if (Projecttile)
    {
        Projecttile->SetShotDirection(Direction);
        Projecttile->SetOwner(GetOwner());
        Projecttile->FinishSpawning(SpawnTransform);
    }

    //UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
    DecreaseAmmo();
    SpawnMuzzleFX();

 }