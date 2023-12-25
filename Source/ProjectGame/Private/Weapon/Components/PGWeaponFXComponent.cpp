// Test Game,  All Rights Reserved.

#include "Weapon/Components/PGWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GamePlayStatics.h"
#include "Components/DecalComponent.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponFXComponentLog, All, All)

UPGWeaponFXComponent::UPGWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPGWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)

{
    auto ImpactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactDataMap[PhysMat];
        }
    }
    // UE_LOG(WeaponFXComponentLog, Display, TEXT("Bone: %s"), *Hit.BoneName.ToString())
    //  niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),  //
        ImpactData.NiagaraEffect,                               //
        Hit.ImpactPoint,                                        //
        Hit.ImpactNormal.Rotation());

    // последний параметр по умолчанию true удаляет системуц из мира
    // decal
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        ImpactData.DecalData.Matirial,  //
        ImpactData.DecalData.Size,      //
        Hit.ImpactPoint,                //
        Hit.ImpactNormal.Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}
