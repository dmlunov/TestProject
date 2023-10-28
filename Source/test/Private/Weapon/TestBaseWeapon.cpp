// Test Game,  All Rights Reserved.


#include "Weapon/TestBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

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
    UE_LOG(BaseWeaponLog, Display, TEXT("Fireeeee"));

}