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
}

void ATestBaseWeapon::StartFire()
{
}

void ATestBaseWeapon::StopFire()
{
}

void ATestBaseWeapon::MakeShot()
{
}

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

void ATestBaseWeapon::MakeHit (FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;
    // ���������� ����������� LineTrace � ����������� ����������
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

