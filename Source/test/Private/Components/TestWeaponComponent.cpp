// Test Game,  All Rights Reserved.

#include "Components/TestWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/TestEquipFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(TestWeaponComponentLog, All, All);

UTestWeaponComponent::UTestWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTestWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentWeaponIndex = 0;
    InitAnimation();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void UTestWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayPeason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();
    Super::EndPlay(EndPlayPeason);
}

void UTestWeaponComponent::SpawnWeapons()
{

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ATestBaseWeapon>(WeaponClass);
        if (!Weapon) continue;
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSoked(Weapon, Character->GetMesh(), WeaponArmorySoketName);
    }
}

void UTestWeaponComponent::AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName)

{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SoketName);
}

void UTestWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponArmorySoketName);
    }
    CurrentWeapon = Weapons[WeaponIndex];
    AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponEquipSoketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipMontage);
}

void UTestWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    CurrentWeapon->StartFire();
}

void UTestWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;

    CurrentWeapon->StopFire();
}

void UTestWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void UTestWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    Character->PlayAnimMontage(Animation);
}

void UTestWeaponComponent::InitAnimation()
{
    if (!EquipMontage) return;

    const auto NotifyEvents = EquipMontage->Notifies;
    for (auto NotifyEvent : NotifyEvents)
    {
        auto EquipFinishedNotify = Cast<UTestEquipFinishedAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishedNotify)
        {
            EquipFinishedNotify->OnNotified.AddUObject(this, &UTestWeaponComponent::OnEquipFinished);
            break;
        }
    }
}
void UTestWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;
    EquipAnimInProgress = false;
}

bool UTestWeaponComponent::CanFire() const 
{
    return CurrentWeapon && !EquipAnimInProgress;
}
bool UTestWeaponComponent::CanEquip() const 
{
    return !EquipAnimInProgress;
}