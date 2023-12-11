// Test Game,  All Rights Reserved.


#include "AI/Tasks/PGNextLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "NavigationSystem.h"

UPGNextLocationTask::UPGNextLocationTask()
{
    NodeName = "Next Locatoin";
};


EBTNodeResult::Type UPGNextLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const auto Controller = OwnerComp.GetAIOwner();
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;
    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
    // UNavigationSystemV1 специальный класс который описывает текущую навигационную систему
    // для получения указатель навигационной системы павна, для создания случайной точки
    if (!NavSys) return EBTNodeResult::Failed;
    FNavLocation NavLocation;
    auto Location = Pawn->GetActorLocation();
    if (!SelfCenter)
    {
        auto CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }

    const auto Found = NavSys->GetRandomReachablePointInRadius(Location, Radius, NavLocation);
    if (!Found) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
    // AimLocationKey.SelectedKeyName имя ключа вектора
    return EBTNodeResult::Succeeded;
}
