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
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count coulds`n be less or equal zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count coulds`n be less or equal zero"));
    CurrentAmmo = DefaultAmmo;
}

void ATestBaseWeapon::StartFire() {}

void ATestBaseWeapon::StopFire() {}

void ATestBaseWeapon::MakeShot() {}

APlayerController* ATestBaseWeapon::GetPlayerController() const
{
    // ��� �� �������� Cast ������ � TestWeaponComponent ������������� CurrentWeapon->SetOwner(Character);
    const auto Pleyer = Cast<ACharacter>(GetOwner());
    if (!Pleyer) return nullptr;

    return Pleyer->GetController<ATestPlayerController>();
}

bool ATestBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;
    // � ������� ���������� �������� ������� � ����������� ������
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
    // ���������� ����������� LineTrace � ����������� ����������
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ATestBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(BaseWeaponLog, Display, TEXT("Clip is empty"));
        return;
    }

    CurrentAmmo.Bullets--;
    LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        // ChangeClip();
        StopFire();
        OnClipEmpty.Broadcast();
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

    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(BaseWeaponLog, Display, TEXT("No more clips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(BaseWeaponLog, Display, TEXT("--- Change Clip ---"));
}

bool ATestBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

void ATestBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo" + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(BaseWeaponLog, Display, TEXT("%s"), *AmmoInfo);
}
