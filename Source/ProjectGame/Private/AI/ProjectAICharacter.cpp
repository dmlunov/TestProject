// Test Game,  All Rights Reserved.


#include "AI/ProjectAICharacter.h"
#include  "AI/PGAIController.h"

AProjectAICharacter::AProjectAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = APGAIController::StaticClass();
}