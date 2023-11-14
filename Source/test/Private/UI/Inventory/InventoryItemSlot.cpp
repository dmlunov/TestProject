// Test Game,  All Rights Reserved.
//game
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "UI/Inventory/ItemDragDropOperation.h"

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
        if (ItemReference->NumericData.bIsStackble && ItemReference->Quantity >1)
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
    FReply Reply =  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }

    // submenu on right click will hapen here
    return Reply.Unhandled();
}

void UInventoryItemSlot::NativeOnDragDetected(  //
    const FGeometry& InGeometry,                //
    const FPointerEvent& InMouseEvent,          //
    UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (DragItemVisualClass)
    {
        const TObjectPtr<UDragItemVisual> DragVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
        DragVisual->ItemIcon->SetBrushFromTexture(ItemReference->AssetData.Icon);
        DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());
        DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));

        UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
        DragItemOperation->SourceItem = ItemReference;
        DragItemOperation->SourceInventory = ItemReference->OwningInventoryComponent;
        DragItemOperation->DefaultDragVisual = DragVisual;
        DragItemOperation->Pivot = EDragPivot::TopLeft;
        OutOperation = DragItemOperation;

    }

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
