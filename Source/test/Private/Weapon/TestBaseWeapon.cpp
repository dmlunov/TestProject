// Test Game,  All Rights Reserved.

#include "Weapon/TestBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/TestPlayerController.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(BaseWeaponLog, All, All);

ATestBaseWeapon::ATestBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
    SetRootComponent(WeaponMesh);
}
void ATestBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
    CurrentAmmo = DefaultAmmo;
}

void ATestBaseWeapon::StartFire() {}

void ATestBaseWeapon::StopFire() {}

void ATestBaseWeapon::MakeShot() {}

APlayerController* ATestBaseWeapon::GetPlayerController() const
{
    // что бы получить Cast прошел в TestWeaponComponent устанавливаем CurrentWeapon->SetOwner(Character);
    const auto Pleyer = Cast<ACharacter>(GetOwner());
    if (!Pleyer) return nullptr;

    return Pleyer->GetController<ATestPlayerController>();
}

bool ATestBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;
    // с помошью контролера получаем позицию и направления камеры
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

FVector ATestBaseWeapon::GetMuzzleWorldLocation() const
{
    // const FTransform SocketTransform = WeaponMesh->GetSocketTransform(SocketName);
    // SocketTransform.GetLocation()
    return WeaponMesh->GetSocketLocation(SocketName);
}

bool ATestBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;

    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = ViewRotation.Vector();  // SocketTransform.GetRotation().GetForwardVector();
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ATestBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;
    // игнарируем пересечения LineTrace с собственным персанажем
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ATestBaseWeapon::DecreaseAmmo()
{
    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        ChangeClip();
    }
}

bool ATestBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

bool ATestBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}
void ATestBaseWeapon::ChangeClip()
{
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    if (!CurrentAmmo.Infinite)
    {
        CurrentAmmo.Clips--;
        UE_LOG(BaseWeaponLog, Display, TEXT("--- Change Clip ---"));

    }

}
void ATestBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo" + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}