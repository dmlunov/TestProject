// Test Game,  All Rights Reserved.


#include "AI/ProjectAICharacter.h"
#include  "AI/PGAIController.h"
#include "ProjectUtils.h"
#include "Components/TestWeaponComponent.h"


AProjectAICharacter::AProjectAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = APGAIController::StaticClass();
}

void AProjectAICharacter::BeginPlay()
{
    Super::BeginPlay();
   

   WeaponComponent;

   // InventoryComponent->AddNewItem(Item, 1);
}