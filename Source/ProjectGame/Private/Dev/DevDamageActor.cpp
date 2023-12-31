// Test Game,  All Rights Reserved.

#include "Dev/DevDamageActor.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Dev/FireDamageType.h"


// Sets default values
ADevDamageActor::ADevDamageActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    UClass* FireDamageType = UFireDamageType::StaticClass();

    DamageType = FireDamageType; 

}

// Called when the game starts or when spawned
void ADevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ADevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, DamageType, {}, this, nullptr, DoFullDamage);
    
    
}
