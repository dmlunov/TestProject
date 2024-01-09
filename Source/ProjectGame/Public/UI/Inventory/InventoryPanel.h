// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanel.generated.h"

class UWrapBox;
class UTextBlock;
class AProjectBaseCharacter;
class UInventoryItemSlot;
class UTestInventoryComponent;


UCLASS()
class PROJECTGAME_API UInventoryPanel : public UUserWidget
{
    GENERATED_BODY()
public:

    UFUNCTION()
    void RefreshInventory();

    UPROPERTY(meta = (BindWidget))
    UWrapBox* InventoryWrapBox;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* WeightInfo;
    UPROPERTY(meta = (BindWidget))
    UTextBlock* CapacityInfo;
    UPROPERTY()
   const AProjectBaseCharacter* BaseCharacter;
    UPROPERTY()
    UTestInventoryComponent* InventoryReference;

    UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
    TSubclassOf<UInventoryItemSlot> InventorySlotClass;

    protected:
    void SetInfoText() const;

    virtual void NativeOnInitialized() override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
