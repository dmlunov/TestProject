// Test Game,  All Rights Reserved.

#include "AI/EQS/PGFriendEnvQueryContext.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(PGFriendEnvQueryContextLog, All, All);

void UPGFriendEnvQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    const auto QueryOwner = Cast<AActor>(QueryInstance.Owner.Get());

    const auto Blackboard = UAIBlueprintHelperLibrary::GetBlackboard(QueryOwner);
    if (!Blackboard) return;
    const auto ContextActor = Blackboard->GetValueAsObject(EnemyActorKeyName);
    const auto ContextActor1 = Cast<AActor>(ContextActor);
    if (ContextActor1) UE_LOG(PGFriendEnvQueryContextLog, Display, TEXT("Friend Context Name = %s"), *ContextActor1->GetName());

    UEnvQueryItemType_Actor::SetContextHelper(ContextData, ContextActor1);
    // скопировали из базовой функции \EnvironmentQuery\Contexts\EnvQueryContext_Querier.cpp
}