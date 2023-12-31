// Test Game,  All Rights Reserved.

#include "Components/TestWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"
#include "GameFramework/Character.h"
#include "Animation/TestEquipFinishedAnimNotify.h"
#include "Animation/TestReloadFinishAnimNotify.h"
#include "Animation/PGPunchFinishAnimNotify.h"
#include "Animation/AnimUtils.h"
#include "Weapon/TestShotgunWeapon.h"
#include "UI/TestGameHUD.h"
#include "AI/ProjectAICharacter.h"
#include "Weapon/TestRifelWeapon.h"
#include "Weapon/PGPunchAttack.h"

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

    //    AProjectAICharacter* AICharacter = Cast<AProjectAICharacter>(GetOwner());
    //  if (AICharacter )  {}

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {

        ATestBaseWeapon* Weapon = GetWorld()->SpawnActor<ATestBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        UE_LOG(TestWeaponComponentLog, Display, TEXT("Weapon name = %s "), *Weapon->GetName());

        Weapon->OnClipEmpty.AddUObject(this, &UTestWeaponComponent::OnEmptyClip);

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        if (Weapon->IsA<ATestShotgunWeapon>() || Weapon->IsA<APGPunchAttack>())  // if (Cast<ATestShotgunWeapon>(Weapon))
            AttachWeaponToSoked(Weapon, Character->GetMesh(), WeaponArmorySoket2);
        else
            AttachWeaponToSoked(Weapon, Character->GetMesh(), WeaponArmorySoketName);
    }

    for (auto weapon : Weapons)
    {
        weapon->SetActorHiddenInGame(true);
        if (weapon->IsWeaponInInventory)
        {
            weapon->SetActorHiddenInGame(false);
            WeaponsInInventary.Add(weapon);
        }
    }
}

void UTestWeaponComponent::AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName)

{
    if (!Weapon || !SceneComponent) return;
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SoketName);
}
void UTestWeaponComponent::FastNextWeapon()
{
    if (WeaponsInInventary.IsEmpty())
        CurrentWeaponIndex = -1;
    else if (WeaponsInInventary.Num() < 1)
        CurrentWeaponIndex = 0;
    else
        CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponsInInventary.Num();
    EquipWeapon(CurrentWeaponIndex);
    // UE_LOG(TestWeaponComponentLog, Display, TEXT("Next waepon index = %i"), CurrentWeaponIndex);
}

void UTestWeaponComponent::NextWeapon()
{
    // UE_LOG(TestWeaponComponentLog, Display, TEXT("Reload Animation Weapons In Inventary = %i"), WeaponsInInventary.Num());
    if (!CanEquip()) return;  //|| WeaponsInInventary.Num() < 1

    if (WeaponsInInventary.IsEmpty())
        CurrentWeaponIndex = -1;
    else if (WeaponsInInventary.Num() < 1)
        CurrentWeaponIndex = 0;
    else
        CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponsInInventary.Num();
    if (WeaponsInInventary.Num() > 1) EquipWeapon(CurrentWeaponIndex);
}

void UTestWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= WeaponsInInventary.Num() || WeaponsInInventary.IsEmpty())
    {
        UE_LOG(TestWeaponComponentLog, Display, TEXT("invalid waepon index"));
        return;
    }
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    int32 LastIndex = ClampIndex(WeaponIndex + 1, 1, WeaponsInInventary.Num() - 1, 0);
    int32 NextIndex = ClampIndex(WeaponIndex, 1, WeaponsInInventary.Num() - 1, 0);

    UE_LOG(TestWeaponComponentLog, Display, TEXT("waepon index = %i, last index = %i, next index = %i"), WeaponIndex, LastIndex, NextIndex);

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        if (CurrentWeapon->IsA<ATestShotgunWeapon>() ||
            CurrentWeapon->IsA<APGPunchAttack>())  // if (Cast<ATestShotgunWeapon>(CurrentWeapon))
            AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponArmorySoket2);
        else
            AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponArmorySoketName);

        CurrentWeapon->SetActorHiddenInGame(false);
    }

    CurrentWeapon = WeaponsInInventary[WeaponIndex];
   
    ATestBaseWeapon* LastWeapon = WeaponsInInventary[LastIndex];
    ATestBaseWeapon* NextWeapon = WeaponsInInventary[NextIndex];

    for (ATestBaseWeapon* weapon : WeaponsInInventary)
    {
        weapon->SetActorHiddenInGame(true);
    }
     CurrentWeapon->SetActorHiddenInGame(false);
    NextWeapon->SetActorHiddenInGame(false);

   // if (WeaponsInInventary.Num() - 1 > 1){
   //     NextWeapon->SetActorHiddenInGame(false);
   //     LastWeapon->SetActorHiddenInGame(true);}
   // 
    // CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData =
        WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });

    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    if (CurrentWeapon->IsA<ATestShotgunWeapon>() || CurrentWeapon->IsA<APGPunchAttack>())  // (Cast<ATestShotgunWeapon>(CurrentWeapon))
    {
        AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponEquipSoketName2);
    }
    else
    {
        AttachWeaponToSoked(CurrentWeapon, Character->GetMesh(), WeaponEquipSoketName);
    }
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipMontage);

    IsGetWeapon = CurrentWeapon->IsA<APGPunchAttack>() ? false : true;
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
        if (!OneWeaponData.ReloadAnimMontage) continue;

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

    return                         //
        CurrentWeapon              //
        && !EquipAnimInProgress    //
        && !ReloadAnimInProgress;  //
                                   // && CurrentWeapon->CanReload();
}

void UTestWeaponComponent::Reload()
{

    if (CanReload()) return;  // WeaponsInInventary.Num() <= 0) return;
    ChangeClip();
}

void UTestWeaponComponent::OnEmptyClip()
{
    ChangeClip();
}

void UTestWeaponComponent::ChangeClip()
{
    if (!CurrentReloadAnimMontage) return;

    ReloadAnimInProgress = true;
    // ReloadAnimInProgress = true;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeClip();
    PlayAnimMontage(CurrentReloadAnimMontage);
}

int32 UTestWeaponComponent::ClampIndex(int32 value, int32 valueStep, int32 max, int32 min) const
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

bool UTestWeaponComponent::GetCurrentWeaponUIDate(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
};

bool UTestWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
};