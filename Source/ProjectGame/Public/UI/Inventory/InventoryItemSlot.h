// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryItemSlot.generated.h"

class UItemBase;
class UDragItemVisual;
class UInventoryTooltip;
class UBorder;
class UImage;
class UTextBlock;
class UInventoryActionMessage;
class UMainMenu;

UCLASS()
class PROJECTGAME_API UInventoryItemSlot : public UUserWidget
{
    GENERATED_BODY()
public:
    FORCEINLINE void SetItemReference(UItemBase* ItemIn) { ItemReference = ItemIn; };
    FORCEINLINE UItemBase* GetItemReference() const { return ItemReference;};

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    TSubclassOf<UDragItemVisual> DragItemVisualClass;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    TSubclassOf<UInventoryTooltip> TooltipClass;

    //UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    //TSubclassOf<UInventoryActionMessage> InventoryActionMessageClass;

    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
    UItemBase* ItemReference;

    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
    UBorder* ItemBorder;

    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
    UTextBlock* ItemQuantity;

    UMainMenu* MainMenuWidget;

    //UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
   // UInventoryActionMessage* ActionMessageWidget;

    FVector2D InitialMousePosition;
    FVector2D FinalMousePosition;

    virtual void NativeOnInitialized() override;

    virtual void NativeConstruct() override;

    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    virtual void NativeOnDragDetected(
        const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
