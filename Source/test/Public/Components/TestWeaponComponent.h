// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectCoreTypes.h"
#include "TestWeaponComponent.generated.h"

class ATestBaseWeapon;
class ATestGameHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UTestWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTestWeaponComponent();
    void StartFire();
    void StopFire();
    void NextWeapon();
    void Reload();

    UFUNCTION()
    TArray<ATestBaseWeapon*> GetWeaponsInInventary() const { return WeaponsInInventary; };

    UFUNCTION()
    void SetWeaponsInInventary(TArray<ATestBaseWeapon*> NewWeapons)
    {
        WeaponsInInventary.Empty();
        WeaponsInInventary = NewWeapons;
    };

    UFUNCTION()
    TArray<ATestBaseWeapon*> GetWeapons() const { return Weapons; };

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayPeason) override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSoketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySoketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipShotGunSoketName = "WeaponSocketToShotGun";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySoketToShotGunName = "ArmorySocketToShotGun";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* EquipMontage;

private:
    UPROPERTY()
    ATestBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ATestBaseWeapon*> Weapons;

    UPROPERTY()
    TArray<ATestBaseWeapon*> WeaponsInInventary;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;

    bool ReloadAnimInProgress = false;

    UPROPERTY()
    ATestGameHUD* TestGameHUD;

    int32 CurrentWeaponIndex = 0;
    int32 LastWeaponIndex = 1;

    void InitAnimation();
    void SpawnWeapons();
    void AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName);
    void EquipWeapon(int32 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* Animation);

    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanFire() const;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();

    int32 ClampIndex(int32 value, int32 valueStep, int32 max, int32 min);
};
