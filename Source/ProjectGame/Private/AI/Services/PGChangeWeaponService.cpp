// Test Game,  All Rights Reserved.


#include "AI/Services/PGChangeWeaponService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ProjectUtils.h"
#include "Components/PGAIWeaponComponent.h"

UPGChangeWeaponService::UPGChangeWeaponService()
{
    NodeName = "Chenge Weapon";
}

void UPGChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (Controller)
    {
        const auto WeaponComponent = Controller->GetPawn()->GetComponentByClass<UPGAIWeaponComponent>();
        //const auto WeaponComponent = ProjectUtils::GetProjectPlayerComponent<UPGAIWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
        {
            WeaponComponent->NextWeapon();
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}