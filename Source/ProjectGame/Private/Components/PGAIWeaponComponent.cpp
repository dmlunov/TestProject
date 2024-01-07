// Test Game,  All Rights Reserved.

#include "Components/PGAIWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"

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
    int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    while (NextIndex != CurrentWeaponIndex)
    {
        if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
        NextIndex = (NextIndex + 1) % Weapons.Num();
    }
    if (CurrentWeaponIndex != NextIndex)
    {
        CurrentWeaponIndex = NextIndex;
        EquipWeapon(CurrentWeaponIndex);
    }
}