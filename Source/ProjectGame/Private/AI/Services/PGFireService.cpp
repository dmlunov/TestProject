// Test Game,  All Rights Reserved.


#include "AI/Services/PGFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ProjectUtils.h"
#include "Components/PGAIWeaponComponent.h"

UPGFireService::UPGFireService() 
{
    NodeName = "Fire";
}

void UPGFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto HasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

    if (Controller)
    {
        const auto WeaponComponent = ProjectUtils::GetProjectPlayerComponent<UPGAIWeaponComponent>(Controller->GetPawn());
        if (WeaponComponent)
        {
            HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}