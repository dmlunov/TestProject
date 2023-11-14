// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/TestBaseInterface.h"
#include "TestPicupActor.generated.h"

class UItemBase;
class ATestBaseCharacter;
//struct FItemData;

UCLASS()
class TEST_API ATestPicupActor : public AActor, public ITestBaseInterface
{
    GENERATED_BODY()

public:
    ATestPicupActor();

    void InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity);

    void InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity);

    FORCEINLINE UItemBase* GetItemData() { return ItemReference; };

    void UpdateInteractableData();
    void TakePickup(const ATestBaseCharacter* TekerCharacter);

    FORCEINLINE UStaticMeshComponent* GetPickupMesh() const { return PickupMesh; };

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup | Components")
    UStaticMeshComponent* PickupMesh;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
    UDataTable* ItemDataTable;



    UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
    FName DesiredItemID;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item reference")
    UItemBase* ItemReference;

    UPROPERTY(EditDefaultsOnly, Category = "Pickup | Item Initialization")
    int32 ItemQuantity;

    UPROPERTY(VisibleAnyWhere, Category = "Pickup | Interaction")
    FInteractableData InstanceInteractableData;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void BeginFocus() override;
    virtual void EndFocus() override;
    virtual void Interact(ATestBaseCharacter* BaseCharacter) override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
