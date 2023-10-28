// Test Game,  All Rights Reserved.

#include "Weapon/TestBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Player/TestPlayerController.h"

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

void ATestBaseWeapon::Fire()
{
    
    MakeShot();
}

void ATestBaseWeapon::MakeShot()
{
    if (!GetWorld()) return;
    // что бы получить Cast прошел в TestWeaponComponent устанавливаем CurrentWeapon->SetOwner(Character);
    const auto Pleyer = Cast<ACharacter>(GetOwner());
    if (!Pleyer) return;

    const auto Controller = Pleyer->GetController<ATestPlayerController>();
    if (!Controller) return;

    // с помошью контролера получаем позицию и направления камеры
    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    const FTransform SocketTransform = WeaponMesh->GetSocketTransform(SocketName);
    const FVector TraceStart = ViewLocation;               // SocketTransform.GetLocation();
    const FVector ShootDirection = ViewRotation.Vector();  // SocketTransform.GetRotation().GetForwardVector();
    const FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    // игнарируем пересечения LineTrace с собственным персанажем
    FCollisionQueryParams CollisionParams; 
    CollisionParams.AddIgnoredActor(GetOwner());

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

    if (HitResult.bBlockingHit)
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
        UE_LOG(BaseWeaponLog, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());
    }
    else
    {
        DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Red, false, 3.0f, 0, 3.0f);
    }

}