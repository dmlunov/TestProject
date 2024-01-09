// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectCoreTypes.h"
#include "TestWeaponComponent.generated.h"

class ATestBaseWeapon;
class ATestGameHUD;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTGAME_API UTestWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTestWeaponComponent();
    virtual void StartFire();
    void StopFire();
    virtual void NextWeapon();
    void FastNextWeapon();
    void Reload();

    bool GetCurrentWeaponUIDate(FWeaponUIData& UIData) const;
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<ATestBaseWeapon> WeaponType, int32 ClipsAmount);
    bool NeedAmmo(TSubclassOf<ATestBaseWeapon> WeaponType);


    // если перс держит огнестрельное орижие то true если рукопашное то false
    bool IsTakeWeapon() const { return IsGetWeapon; };

    bool InvntaryEquipWeapon = false;

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
    UFUNCTION()
    ATestBaseWeapon* GetCurrentWeapon() const { return CurrentWeapon; };
    UFUNCTION()
    void SetCurrentWeapon(ATestBaseWeapon* NewCurrentWeapon) { CurrentWeapon = NewCurrentWeapon; };

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
    FName WeaponEquipSoketName2 = "WeaponSocket2";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySoket2 = "ArmorySocket2";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UAnimMontage* EquipMontage;

    UPROPERTY()
    ATestBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ATestBaseWeapon*> Weapons;

    UPROPERTY()
    TArray<ATestBaseWeapon*> WeaponsInInventary;

    bool CanFire() const;

    bool CanEquip() const;

    void EquipWeapon(int32 WeaponIndex);

    int32 CurrentWeaponIndex = 0;

private:
    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;

    bool ReloadAnimInProgress = false;

    bool IsGetWeapon = false;

    UPROPERTY()
    ATestGameHUD* TestGameHUD;

    int32 LastWeaponIndex = 1;

    void InitAnimation();
    void SpawnWeapons();
    void AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName);

    void PlayAnimMontage(UAnimMontage* Animation);

    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanReload() const;

    void OnEmptyClip(ATestBaseWeapon* AmmoEmptyWeapon);
    void ChangeClip();

    int32 ClampIndex(int32 value, int32 valueStep, int32 max, int32 min) const;


};
