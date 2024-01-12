// Test Game,  All Rights Reserved.


#include "AI/Services/PGFindFriendService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Player/TestPlayerController.h"
#include "ProjectUtils.h"
#include "Components/PGAIPerceptionComponent.h"
#include "Components/HelthComponent.h"
#include "ProjectUtils.h"


DEFINE_LOG_CATEGORY_STATIC(PGFindFriendServiceLog, All, All);


UPGFindFriendService::UPGFindFriendService()
{
    NodeName = "Finde Friend";  // NodeName название сервиса
}

void UPGFindFriendService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
   
    UE_LOG(PGFindFriendServiceLog, Display, TEXT("Find frend "));
    const auto Blackboard = OwnerComp.GetBlackboardComponent();
    const auto AIController = OwnerComp.GetAIOwner();

     for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
    {
        const auto Controller = It->Get();
        if (!Controller) continue;
        const auto PlayerController = Cast<ATestPlayerController>(Controller);
        if (!PlayerController) continue;
        const auto FriendPawn = PlayerController->GetPawn();
        if (!FriendPawn) continue;
        const auto HealthComponent = FriendPawn->GetComponentByClass<UHelthComponent>();
        const auto AreEnemies =  ProjectUtils::AreEnemies(AIController, PlayerController);
        const auto FriendActor = Cast<AActor>(FriendPawn);
        if (HealthComponent && !HealthComponent->IsDead() && !AreEnemies && Blackboard)  //
        {
            UE_LOG(PGFindFriendServiceLog, Display, TEXT("Find frend = %s"), *FriendActor->GetName());
            Blackboard->SetValueAsObject(FriendActorKey.SelectedKeyName, FriendActor);
            break;
        }
    }
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
