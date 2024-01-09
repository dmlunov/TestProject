// Test Game,  All Rights Reserved.

#include "Components/PGAIWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"
#include "Weapon/PGPunchAttack.h"

void UPGAIWeaponComponent::BeginPlay()
{
    Super::BeginPlay();
    NextWeapon();
}


void UPGAIWeaponComponent ::StartFire()
{
    if (!CanFire()) return;
    if (CurrentWeapon->IsAmmoEmpty())
    {
        NextWeapon();
    }
    else
    {
        CurrentWeapon->StartFire();
    }
}

void UPGAIWeaponComponent ::NextWeapon()
{
    if (!CanEquip()) return;

    int32 NextIndex = (CurrentWeaponIndex + 1) % WeaponsInInventary.Num();
    int32 PunchAttack = NULL;
    int32 count = 0;
    while (NextIndex != CurrentWeaponIndex)
    {
        if (PunchAttack == NextIndex) break;

        if ( WeaponsInInventary[NextIndex]->IsA<APGPunchAttack>())
        {
            PunchAttack = NextIndex;
            continue;
        }

        if (!WeaponsInInventary[NextIndex]->IsAmmoEmpty()) break;

        NextIndex = (NextIndex + 1) % WeaponsInInventary.Num();
        count++;
        if (count > WeaponsInInventary.Num())
        {
            NextIndex = PunchAttack;
            break;
        }

    }
    if (CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}


