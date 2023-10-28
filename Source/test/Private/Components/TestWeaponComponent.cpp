// Test Game,  All Rights Reserved.

#include "Components/TestWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"
#include "GameFramework/Character.h"

UTestWeaponComponent::UTestWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTestWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();
}

void UTestWeaponComponent::SpawnWeapon()
{
    if (!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    CurrentWeapon = GetWorld()->SpawnActor<ATestBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
}

void UTestWeaponComponent::Fire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->Fire();

}