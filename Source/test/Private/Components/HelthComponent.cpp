// Test Game,  All Rights Reserved.


#include "Components/HelthComponent.h"


UHelthComponent::UHelthComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UHelthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
}




