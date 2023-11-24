// Test Game,  All Rights Reserved.
// game
#include "UI/Inventory/InventoryPanel.h"
#include "Player/TestBaseCharacter.h"
#include "Components/TestInventoryComponent.h"
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/ItemDragDropOperation.h"
// engine
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

void UInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    BaseCharacter = Cast<ATestBaseCharacter>(GetOwningPlayerPawn());
    if (BaseCharacter)
    {
        InventoryReference = BaseCharacter->GetInventoryComponent();
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
        InventoryWrapBox->ClearChildren();
        for (UItemBase* const& InventeryItem : InventoryReference->GetInventoryContents())
        {
            UInventoryItemSlot* ItemSlot = CreateWidget<UInventoryItemSlot>(this, InventorySlotClass);
            ItemSlot->SetItemReference(InventeryItem);
            InventoryWrapBox->AddChildToWrapBox(ItemSlot);
        }
        SetInfoText();
    }
}

void UInventoryPanel::SetInfoText() const
{
    const FString WeightInfoValue{FString::SanitizeFloat(InventoryReference->GetInventoryTotalWeight()) + "/" +  //
                                  FString::SanitizeFloat(InventoryReference->GetWeightCapacity())};

    const FString CapacityInfoValue{FString::SanitizeFloat(InventoryReference->GetInventoryContents().Num()) + "/" +  //
                                    FString::SanitizeFloat(InventoryReference->GetSloatCapacity())};

    WeightInfo->SetText(FText::FromString(WeightInfoValue));
    CapacityInfo->SetText(FText::FromString(CapacityInfoValue));
}

bool UInventoryPanel::NativeOnDrop(         //
    const FGeometry& InGeometry,            //
    const FDragDropEvent& InDragDropEvent,  //
    UDragDropOperation* InOperation)
{
    const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
    if (ItemDragDrop->SourceItem && InventoryReference)
    {
    
        return true;
    }

    return false;
   // return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
