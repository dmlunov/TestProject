// Test Game,  All Rights Reserved.

#include "Components/TestWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/TestEquipFinishedAnimNotify.h"
#include "Animation/TestReloadFinishAnimNotify.h"
#include "Animation/AnimUtils.h"
#include "Weapon/TestShotgunWeapon.h"
#include "UI/TestGameHUD.h"


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
    TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

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

    for (auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ATestBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnClipEmpty.AddUObject(this, &UTestWeaponComponent::OnEmptyClip);

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

         if (Weapon->IsA<ATestShotgunWeapon>())  // if (Cast<ATestShotgunWeapon>(Weapon))
            AttachWeaponToSoked(Weapon, Character->GetMesh(), WeaponArmorySoketToShotGunName);
        else
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
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(TestWeaponComponentLog, Display, TEXT("invalid waepon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    int32 LastIndex = ClampIndex(WeaponIndex + 1, 1, Weapons.Num() - 1, 0);
    int32 NextIndex = ClampIndex(WeaponIndex, 1, Weapons.Num() - 1, 0);

    // UE_LOG(TestWeaponComponentLog, Display, TEXT("waepon index = %i, last index = %i, nwxt index = %i"), WeaponIndex, LastIndex,
    // NextIndex);

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        if (CurrentWeapon->IsA<ATestShotgunWeapon>())  // if (Cast<ATestShotgunWeapon>(CurrentWeapon))
            AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponArmorySoketToShotGunName);
        else
            AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponArmorySoketName);
        CurrentWeapon->SetActorHiddenInGame(false);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    CurrentWeapon->SetActorHiddenInGame(false);
    ATestBaseWeapon* LastWeapon = Weapons[NextIndex];
    LastWeapon->SetActorHiddenInGame(true);
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData =
        WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });

    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    if (CurrentWeapon->IsA<ATestShotgunWeapon>())  // (Cast<ATestShotgunWeapon>(CurrentWeapon))
    {
        AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponEquipShotGunSoketName);
    }
    else
    {
        AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponEquipSoketName);
    }
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
    auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UTestEquipFinishedAnimNotify>(EquipMontage);
    if (EquipFinishedNotify)
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &UTestWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(TestWeaponComponentLog, Error, TEXT("Equip Animation notify is forgotten to set"));
        checkNoEntry();
    }

    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishAnimMontage = AnimUtils::FindNotifyByClass<UTestReloadFinishAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishAnimMontage)
        {
            UE_LOG(TestWeaponComponentLog, Error, TEXT("Reload Animation notify is forgotten to set"));
            checkNoEntry();
        }
        ReloadFinishAnimMontage->OnNotified.AddUObject(this, &UTestWeaponComponent::OnReloadFinished);
    }
}

void UTestWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;
    EquipAnimInProgress = false;
}

void UTestWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;
    ReloadAnimInProgress = false;
}

bool UTestWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && !TestGameHUD->bIsMenuVisible;
}
bool UTestWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UTestWeaponComponent::CanReload() const
{
    return CurrentWeapon             //
           && !EquipAnimInProgress   //
           && !ReloadAnimInProgress  //
           && CurrentWeapon->CanReload();
}

void UTestWeaponComponent::Reload()
{
    ChangeClip();
}

void UTestWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void UTestWeaponComponent::ChangeClip()
{
    ReloadAnimInProgress = true;
    ReloadAnimInProgress = true;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    PlayAnimMontage(CurrentReloadAnimMontage);
}

int32 UTestWeaponComponent::ClampIndex(int32 value, int32 valueStep, int32 max, int32 min)
{
    value += valueStep;
    int32 out;
    if (value > max)
        out = value % (max + 1);
    else if (value < min)
        out = (value + 1) % (max + 1) + max;
    else
        out = value;
    return out;
}
