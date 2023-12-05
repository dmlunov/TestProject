// Test Game,  All Rights Reserved.
// game
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/Inventory/DragItemVisual.h"
#include "UI/Inventory/InventoryTooltip.h"
#include "Items/ItemBase.h"
#include "UI/Inventory/ItemDragDropOperation.h"
#include "UI/Inventory/InventoryActionMessage.h"
#include "UI/TestGameHUD.h"
#include "UI/MainMenu.h"
// engine
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(InventoryItemSlotLog, All, All);
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
        if (ItemReference->NumericData.bIsStackble && ItemReference->Quantity > 1)
        {
            ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity));
        }
        else
        {
            ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
        }
    }

    ATestGameHUD* TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    MainMenuWidget = TestGameHUD->GetMainMenuWidget();

    // MainMenuWidget->InventoryActionMessage = this;
}

FReply UInventoryItemSlot::NativeOnMouseButtonDown(  //
    const FGeometry& InGeometry,                     //
    const FPointerEvent& InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        InitialMousePosition = InMouseEvent.GetScreenSpacePosition();

        // UE_LOG(InventoryItemSlotLog, Display, TEXT("InventoryItemSlotLog OnMouseButtonDown"));
        return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }

    // submenu on right click will hapen here
    return Reply.Unhandled();
}

FReply UInventoryItemSlot::NativeOnMouseButtonUp(  //
    const FGeometry& InGeometry,                   //
    const FPointerEvent& InMouseEvent)
{
    FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
    //
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        FinalMousePosition = InMouseEvent.GetScreenSpacePosition();

        float DeltaX = FinalMousePosition.X - InitialMousePosition.X;
        float DeltaY = FinalMousePosition.Y - InitialMousePosition.Y;
        if (FMath::IsNearlyZero(DeltaX, 0.1f) && FMath::IsNearlyZero(DeltaY, 0.1f))
        {
            //UE_LOG(InventoryItemSlotLog, Display, TEXT("Clic MouseButton"));
            UInventoryActionMessage*  ActionMessageWidget = MainMenuWidget->GetInventoryActionMessageWidget();
            if (!ActionMessageWidget)
            {
                UE_LOG(InventoryItemSlotLog, Display, TEXT(" InventoryActionMessage is emty"));
                return Reply.Unhandled();
            }

            // const TObjectPtr<UInventoryActionMessage>
            ActionMessageWidget->InventorySlot = this;
            ActionMessageWidget->ChangeData();
            // if (ActionMessageWidget->IsVisible())
            ActionMessageWidget->SetVisibility(ESlateVisibility::Visible);
            //UE_LOG(InventoryItemSlotLog, Display, TEXT("ActionMessageWidget can be Visible"));

            // ActionMessageWidget->SetVisibility(ESlateVisibility::Visible);
        }

        return Reply.Unhandled();  //.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
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

        ItemReference->NumericData.bIsStackble  //
            ? DragVisual->ItemQuantity->SetText(FText::AsNumber(ItemReference->Quantity))
            : DragVisual->ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);

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
