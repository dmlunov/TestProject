// Test Game,  All Rights Reserved.


#include "AI/Decorators/PGNeedAmmoDecorator.h"
#include "AI/PGAIController.h"
#include "Components/TestWeaponComponent.h"

UPGNeedAmmoDecorator::UPGNeedAmmoDecorator()
{
    NodeName = "Need Ammo";
}

bool UPGNeedAmmoDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;
    const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<UTestWeaponComponent>();
   
    if (!WeaponComponent) return false;

    return WeaponComponent->NeedAmmo(WeaponType);
}
