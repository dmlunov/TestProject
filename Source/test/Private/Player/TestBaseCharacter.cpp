// Test Game,  All Rights Reserved.


#include "Player/TestBaseCharacter.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATestBaseCharacter::ATestBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ATestBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

