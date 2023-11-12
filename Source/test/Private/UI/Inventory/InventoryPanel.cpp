// Test Game,  All Rights Reserved.
//game
#include "UI/Inventory/InventoryPanel.h"
#include "Player/TestBaseCharacter.h"
#include "Components/TestInventoryComponent.h"
#include "UI/Inventory/InventoryItemSlot.h"
//engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"



void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BaseCharacter = Cast<ATestBaseCharacter>(GetOwningPlayerPawn());
    if (BaseCharacter)
    {
        InventoryReference = BaseCharacter->GetInventory();
        if (InventoryReference)
        {
            InventoryReference->OnInventoryUpdate.AddUObject(this, &UInventoryPanel::RefreshInventory);
            SetInfoText();
        }
    }
}

void UInventoryPanel::RefreshInventory()
{
    if (InventoryReference && InventorySlotClass)
    {
        InventoryPanel->ClearChildren();
        for (UItemBase* const& InventeryItem : InventoryReference->GetInventoryContents())
        {
            UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
            ItemSlot->SetItemReference(InventeryItem);
            InventoryPanel->AddChildToWrapBox(ItemSlot);
        }
        SetInfoText();
    }
    
}

void UInventoryPanel::SetInfoText() const
{
    WeightInfo->SetText(FText::Format(
        FText::FromString("{0}/{1}"), InventoryReference->GetInventoryTotalWeight(), InventoryReference->GetWeightCapacity()));
    CapacityInfo->SetText(FText::Format(
        FText::FromString("{0}/{1}"), InventoryReference->GetInventoryContents().Num(), InventoryReference->GetSloatCapacity()));
}

bool UInventoryPanel::NativeOnDrop(         //
    const FGeometry& InGeometry,            //
    const FDragDropEvent& InDragDropEvent,  //
    UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
