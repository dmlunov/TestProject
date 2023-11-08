// Test Game,  All Rights Reserved.

#include "Environment/TestPicupActor.h"
#include "Items/ItemBase.h"
#include "ProjectCoreTypes.h"
#include "CoreMinimal.h"

ATestPicupActor::ATestPicupActor()
{
    PrimaryActorTick.bCanEverTick = false;

    PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
    PickupMesh->SetSimulatePhysics(true);
    SetRootComponent(PickupMesh);
}

void ATestPicupActor::BeginPlay()
{
    Super::BeginPlay();

    InitializePickup(UItemBase::StaticClass(), ItemQuantity);
    // Interact(ATestBaseCharacter * BaseCharacter);
}

void ATestPicupActor::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
    if (ItemDataTable && !DesiredItemID.IsNone())
    {
        const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

        ItemReference = NewObject<UItemBase>(this, BaseClass);

        ItemReference->ID = ItemData->ID;
        ItemReference->ItemType = ItemData->ItemType;
        ItemReference->NumericData = ItemData->ItemNumericData;
        ItemReference->TextData = ItemData->ItemTextData;
        ItemReference->AssetData = ItemData->ItemAssetData;

        InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);

        PickupMesh->SetStaticMesh(ItemData->ItemAssetData.StaticMesh);
        UpdateInteractableData();
    }
}

void ATestPicupActor::UpdateInteractableData()
{
    // EInteractType::Pickup;
    InstanceInteractableData.InteractType = EInteractType::Pickup;
    InstanceInteractableData.Action = ItemReference->TextData.InteractionText;
    InstanceInteractableData.Name = ItemReference->TextData.Name;
    InstanceInteractableData.Quantity = ItemReference->Quantity;
    InteractableData = InstanceInteractableData;
}

void ATestPicupActor::InitializeDrop(UItemBase* ItemToDrop, const int32 InQuantity)
{
    ItemReference = ItemToDrop;
    InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
    ItemReference->NumericData.Weight = ItemToDrop->GetItemSingleWeight();
    PickupMesh->SetStaticMesh(ItemToDrop->AssetData.StaticMesh);
    UpdateInteractableData();
}

void ATestPicupActor::BeginFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(true);
    }
}
void ATestPicupActor::EndFocus()
{
    if (PickupMesh)
    {
        PickupMesh->SetRenderCustomDepth(false);
    }
}
void ATestPicupActor::Interact(ATestBaseCharacter* BaseCharacter)
{
    if (BaseCharacter)
    {
        TakePickup(BaseCharacter);
    }
}

void ATestPicupActor::TakePickup(const ATestBaseCharacter* TekerCharacter)
{
    if (!IsPendingKillPending())
    {
        if (ItemReference)
        {
            // if (UTestItemComponent* PlayerInventory = Taker->GetInventory())
        }
    }
}

void ATestPicupActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

   // const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetName() : NAME_None;
    const FName ChangedPropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

    if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(ATestPicupActor, DesiredItemID))
    {
        if (ItemDataTable)
        {
            
            if (const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString()))
            {
                PickupMesh->SetStaticMesh(ItemData->ItemAssetData.StaticMesh);
            }

        }
    }
}