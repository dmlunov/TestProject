// Test Game,  All Rights Reserved.


#include "Components/TestCharacterMovementComponent.h"
#include "Player/ProjectBaseCharacter.h"

 float UTestCharacterMovementComponent::GetMaxSpeed() const 
 {
     const float MaxSpeed = Super::GetMaxSpeed();
     const AProjectBaseCharacter* Player = Cast<AProjectBaseCharacter>(GetPawnOwner());
     return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
 };
