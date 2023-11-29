// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TestBaseInterface.h"
#include "InterfaceTestActor.generated.h"

class AProjectBaseCharacter;
struct FInteractableData;


UCLASS()
class PROJECTGAME_API AInterfaceTestActor : public AActor, public ITestBaseInterface
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AInterfaceTestActor();

protected:
    UPROPERTY(EditAnyWhere, Category = "Test Actor")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditInstanceOnly, Category = "Test Actor")
    FInteractableData InstanceInteractableData; 

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    virtual void BeginFocus() override;
    virtual void EndFocus() override;
    virtual void BeginInteract() override;
    virtual void EndInteract() override;
    virtual void Interact(AProjectBaseCharacter* BaseCharacter) override;

  
};
