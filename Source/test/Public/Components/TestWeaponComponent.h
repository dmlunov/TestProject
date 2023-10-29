// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestWeaponComponent.generated.h"

class ATestBaseWeapon;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UTestWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTestWeaponComponent();

    UFUNCTION()
    void StartFire();

    UFUNCTION()
    void StopFire();

    void NextWeapon();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayPeason) override;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<TSubclassOf<ATestBaseWeapon>> WeaponClasses;

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

    bool EquipAnimInProgress = false;


    int32 CurrentWeaponIndex;

    void SpawnWeapons();


    void AttachWeaponToSoked(ATestBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SoketName);
    void EquipWeapon(int32 WeaponIndex);

    void PlayAnimMontage(UAnimMontage* Animation);
    void InitAnimation();
    void OnEquipFinished(USkeletalMeshComponent* MeshComp);

    bool CanFire() const ;
    bool CanEquip() const;


};
