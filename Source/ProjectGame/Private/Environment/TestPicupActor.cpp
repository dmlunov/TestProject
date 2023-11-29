// Test Game,  All Rights Reserved.

#include "Environment/TestPicupActor.h"
#include "Items/ItemBase.h"
#include "ProjectCoreTypes.h"
#include "CoreMinimal.h"
#include "Player/ProjectBaseCharacter.h"
#include "Components/TestItemComponent.h"
#include "Components/TestInventoryComponent.h"

DEFINE_LOG_CATEGORY_STATIC(TestPicupActorLog, All, All);

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
    // Interact(AProjectBaseCharacter * BaseCharacter);

   // UE_LOG(TestPicupActorLog, Display, TEXT("PicapActor Create"));
}

void ATestPicupActor::InitializePickup(const TSubclassOf<UItemBase> BaseClass, const int32 InQuantity)
{
    if (ItemDataTable && !DesiredItemID.IsNone())
    {
        FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

        // UE_LOG(TestPicupActorLog, Display, TEXT("Transform Scale = %s"), *ItemData.GetScale3D().ToString());

        ItemReference = NewObject<UItemBase>(this, BaseClass);

        ItemReference->ID = ItemData->ID;
        ItemReference->ItemType = ItemData->ItemType;
        ItemReference->NumericData = ItemData->ItemNumericData;
        ItemReference->TextData = ItemData->ItemTextData;
        ItemReference->AssetData = ItemData->ItemAssetData;
        ItemReference->Transform = PickupMesh->GetComponentTransform();
        ItemReference->ItemPhysicalMass = PickupMesh->GetMass();
 
        ItemReference->NumericData.bIsStackble = ItemData->ItemNumericData.MaxStackSize > 1;
       // ItemData->Transform = PickupMesh->GetComponentTransform();
        InQuantity <= 0 ? ItemReference->SetQuantity(1) : ItemReference->SetQuantity(InQuantity);
        // UE_LOG(TestPicupActorLog, Display, TEXT("Ouantity %s = %i"), *ItemReference->TextData.Name.ToString(), InQuantity);

        PickupMesh->SetStaticMesh(ItemData->ItemAssetData.StaticMesh);

        //ItemData->IsCanChangeTrancform = true;
        //ItemReference->IsCanChangeTrancform = true;

        UpdateInteractableData();
    }
    /* if (ItemDataTable && DesiredItemID.IsNone())
    {
        FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

        if (ItemData->IsCanChangeTrancform)
        {
            PickupMesh->SetRelativeTransform(ItemData->Transform);
            UE_LOG(TestPicupActorLog, Display, TEXT("Transform Scale =  %s"), *ItemData->Transform.GetScale3D().ToString());
        }
    }
    UE_LOG(TestPicupActorLog, Display, TEXT("ItemDataTable =  %s"), ItemDataTable ? TEXT("true") : TEXT("false"));
    UE_LOG(TestPicupActorLog, Display, TEXT("DesiredItemID is %s"), DesiredItemID.IsNone() ? TEXT("true") : TEXT("false"));*/
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
    ItemReference->OwningInventoryComponent = nullptr;
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
void ATestPicupActor::Interact(AProjectBaseCharacter* BaseCharacter)
{
    if (BaseCharacter)
    {
        TakePickup(BaseCharacter);
    }
}

void ATestPicupActor::TakePickup(const AProjectBaseCharacter* TekerCharacter)
{
    if (!IsPendingKillPending())
    {
        if (ItemReference)
        {
            if (UTestInventoryComponent* PlayerInventory = TekerCharacter->GetInventoryComponent())
            {
                const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

                switch (AddResult.OperationResult)
                {
                    case EItemAssResult::IAR_NoItemAdded: break;
                    case EItemAssResult::IAR_PatialAmountItemAdded:
                        UpdateInteractableData();
                        TekerCharacter->GetItemComponent()->UpdateInteractionWidget();
                        break;
                    case EItemAssResult::IAR_AllItemAdded: Destroy(); break;
                }
                UE_LOG(TestPicupActorLog, Display, TEXT("%s"), *AddResult.ResultMessage.ToString());
            }
            else
            {
                UE_LOG(TestPicupActorLog, Display, TEXT("Player Inventory component is null!"));
            }
        }
        else
        {
            UE_LOG(TestPicupActorLog, Display, TEXT("Pickup internal item reference was somehow null!"));
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