// Test Game,  All Rights Reserved.


#include "AI/Decorators/PGHealthPercentDecorator.h"
#include "AI/PGAIController.h"
#include "ProjectUtils.h"
#include "Components/HelthComponent.h"

UPGHealthPercentDecorator::UPGHealthPercentDecorator()
{
    NodeName = "Health Percent";
}

bool UPGHealthPercentDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const auto Controller = OwnerComp.GetAIOwner();
    if (!Controller) return false;
    const auto HealthComponent = Controller->GetPawn()->GetComponentByClass<UHelthComponent>();
    //STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(Controller->GetPawn());
    if (!HealthComponent || HealthComponent->IsDead()) return false;
    return HealthComponent->GetHealthPercent() <= HealthPercent;
}