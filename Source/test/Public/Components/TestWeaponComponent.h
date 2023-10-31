// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectCoreTypes.h"
#include "TestWeaponComponent.generated.h"


class ATestBaseWeapon;





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
    UAnimMontage* EquipMontage;

private:
    UPROPERTY()
    ATestBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<ATestBaseWeapon*> Weapons;

    UPROPERTY()
    UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EquipAnimInProgress = false;

    bool ReloadAnimInProgress = false;

    int32 CurrentWeaponIndex = 0;

    void InitAnimation();
    void SpawnWeapons();
    void AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName);
    void EquipWeapon(int32 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* Animation);
   
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(USkeletalMeshComponent* MeshComp);

    bool CanFire() const ;
    bool CanEquip() const;
    bool CanReload() const;

    void OnEmptyClip();
    void ChangeClip();

   
};
