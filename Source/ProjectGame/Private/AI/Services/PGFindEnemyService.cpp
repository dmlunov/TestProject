// Test Game,  All Rights Reserved.


#include "AI/Services/PGFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ProjectUtils.h"
#include "Components/PGAIPerceptionComponent.h"

UPGFindEnemyService::UPGFindEnemyService()
{
    NodeName = "Finde Enemy";  // NodeName название сервиса
}

void UPGFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    // UE_LOG(LogTemp, Display, TEXT("Find enrmy"));

    const auto Blackboard = OwnerComp.GetBlackboardComponent();

    if (Blackboard)
    {
        const auto Controller = OwnerComp.GetAIOwner();
        const auto PerceptionComponent = ProjectUtils::GetProjectPlayerComponent<UPGAIPerceptionComponent>(Controller);
        if (PerceptionComponent)
        {
            Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosesEnemy());
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}