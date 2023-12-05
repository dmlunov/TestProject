// Test Game,  All Rights Reserved.


#include "UI/MainMenu.h"
#include "Player/ProjectBaseCharacter.h"
#include "UI/Inventory/ItemDragDropOperation.h"
#include "Components/TestItemComponent.h"
#include "Items/ItemBase.h"
#include "UI/Inventory/InventoryActionMessage.h"
#include "UI/Inventory/InventoryPanel.h"


void UMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();

}

void UMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    BaseCharacter = Cast<AProjectBaseCharacter>(GetOwningPlayerPawn());


    InventoryActionMessage->SetVisibility(ESlateVisibility::Collapsed);
}

bool UMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    //return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    // cast operation to item drag drop, ensure player is valid, call drop item on player
    const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);

    if (BaseCharacter && ItemDragDrop->SourceItem)
    {
        BaseCharacter->GetItemComponent()->DropItem(ItemDragDrop->SourceItem, ItemDragDrop->SourceItem->Quantity);
        return true;
    }
    return false;
}