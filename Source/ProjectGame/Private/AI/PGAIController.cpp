// Test Game,  All Rights Reserved.

#include "AI/PGAIController.h"
#include "AI/ProjectAICharacter.h"
#include "Components/PGAIPerceptionComponent.h"
// #include "Components/STURespawnComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

APGAIController::APGAIController()
{
    PGAIPerceptionComponent = CreateDefaultSubobject<UPGAIPerceptionComponent>("PGAIPerceptionComponent");
    SetPerceptionComponent(*PGAIPerceptionComponent);
    // для AI SetPerceptionComponent , для других Actors SetupAttachment(GetRootComponent());
    // RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("USTURespawnComponent");

    bWantsPlayerState = true;
}

void APGAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    const auto PGCHaracter = Cast<AProjectAICharacter>(InPawn);
    if (PGCHaracter)
    {
        RunBehaviorTree(PGCHaracter->BehaviorTreeAsset);
    }
}

void APGAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
     const auto AimActor = GetFocusOnActor();  
     SetFocus(AimActor);
}

AActor* APGAIController::GetFocusOnActor() const
{
   // return  PGAIPerceptionComponent->GetClosesEnemy();
    if (!GetBlackboardComponent()) return nullptr;
    return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
    // GetBlackboardComponent() эта функция возвращает указатель на тип UObject поэтому ее нужно скастить на AActor
}