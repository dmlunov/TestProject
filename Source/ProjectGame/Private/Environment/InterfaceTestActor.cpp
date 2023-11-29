// Test Game,  All Rights Reserved.

#include "Environment/InterfaceTestActor.h"
#include "Interface/TestBaseInterface.h"


DEFINE_LOG_CATEGORY_STATIC(TestIntrfeceActorLog, All, All);
// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    SetRootComponent(Mesh);
    Mesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
    Super::BeginPlay();

    InteractableData = InstanceInteractableData;
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AInterfaceTestActor::BeginFocus()
{
    if (Mesh)
    {
        Mesh->SetRenderCustomDepth(true);
    }
}
void AInterfaceTestActor::EndFocus()
{
    if (Mesh)
    {
        Mesh->SetRenderCustomDepth(false);
    }
}
void AInterfaceTestActor::BeginInteract()
{
    UE_LOG(TestIntrfeceActorLog, Display, TEXT("Intrfece calling *BeginInteract* on test actor"));
}
void AInterfaceTestActor::EndInteract()
{
    UE_LOG(TestIntrfeceActorLog, Display, TEXT("Intrfece calling *EndInteract* on test actor"));
}
void AInterfaceTestActor::Interact(AProjectBaseCharacter* BaseCharacter)
{
    UE_LOG(TestIntrfeceActorLog, Display, TEXT("Intrfece calling *Interact* on test actor"));
}