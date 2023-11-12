// Test Game,  All Rights Reserved.
//game
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"

//engine
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"

void UInventoryItemSlot::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (TooltipClass)
    {
        UInventoryTooltip* ToolTip = CreateWidget<UInventoryTooltip>(this, TooltipClass);
        ToolTip->InventorySlotBeingHovered = this;
        SetToolTip(ToolTip);


    }

    //
}

void UInventoryItemSlot::NativeConstruct()
{
    Super::NativeConstruct();
    if (ItemReference)
    {
        ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
        if (ItemReference->NumericData.bIsStackble)
        {
            ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
        }
        else
        {
            ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(  //
    const FGeometry& InGeometry,                     //
    const FPointerEvent& InMouseEvent)
{
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    //
}

void UInventoryItemSlot::NativeOnDragDetected(  //
    const FGeometry& InGeometry,                //
    const FPointerEvent& InMouseEvent,          //
    UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
    //
}

void UInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    //
}

bool UInventoryItemSlot::NativeOnDrop(      //
    const FGeometry& InGeometry,            //
    const FDragDropEvent& InDragDropEvent,  //
    UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
    //
}
