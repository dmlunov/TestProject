// Test Game,  All Rights Reserved.


#include "AI/EQS/PGEnemyEnvQueryContext.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void UPGEnemyEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;
    const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);
    const auto ContextActor1 = Cast<AActor>(ContextActor);
    UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor1);
    // скопировали из базовой функции \EnvironmentQuery\Contexts\EnvQueryContext_Querier.cpp
}