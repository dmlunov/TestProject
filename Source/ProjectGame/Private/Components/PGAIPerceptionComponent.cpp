// Test Game,  All Rights Reserved.


#include "Components/PGAIPerceptionComponent.h"
#include "AIController.h"
#include "ProjectUtils.h"
#include "Components/HelthComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Damage.h"

 
AActor* UPGAIPerceptionComponent::GetClosesEnemy() const
{
    TArray<AActor*> PercieveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercieveActors);
    // функция которая определяет всех акторов определенным чувством
    if (PercieveActors.Num() == 0) return nullptr;


    const auto Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const auto Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;
   
    float BestDistance = MAX_FLT;
    AActor* BestPawn = nullptr;
    for (const auto PercieveActor : PercieveActors)
    {
        const auto HealthComponent = PercieveActor->GetComponentByClass<UHelthComponent>();
        //const auto HealthComponent = ProjectUtils::GetProjectPlayerComponent<UHelthComponent>(PercieveActor);

        const auto PercievPawn = Cast<APawn>(PercieveActor);
      //  const auto AreEnemies = PercievPawn && ProjectUtils::AreEnemies(Controller, PercievPawn->Controller);
        if (HealthComponent && !HealthComponent->IsDead() ) //&& AreEnemies)  //
        {
            const auto CurrentDistance = (PercieveActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < BestDistance)
            {
                BestDistance = CurrentDistance;
                BestPawn = PercieveActor;
            }
        }
    }

    return BestPawn;
}
