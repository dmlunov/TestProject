// Test Game,  All Rights Reserved.


#include "Components/TestCharacterMovementComponent.h"
#include "Player/TestBaseCharacter.h"

 float UTestCharacterMovementComponent::GetMaxSpeed() const 
 {
     const float MaxSpeed = Super::GetMaxSpeed();
     const ATestBaseCharacter* Player = Cast<ATestBaseCharacter>(GetPawnOwner());
     return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
 };
