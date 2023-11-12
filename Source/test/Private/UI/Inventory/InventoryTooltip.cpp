// Test Game,  All Rights Reserved.


#include "UI/Inventory/InventoryTooltip.h"
#include "UI/Inventory/InventoryItemSlot.h"
#include "Items/ItemBase.h"

#include "Components/TextBlock.h"


void UInventoryTooltip ::NativeConstruct()
{
    Super::NativeConstruct();

    UItemBase* ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();

    switch (ItemBeingHovered->ItemType)
    {
        case EItemType::Armor: break;
        case EItemType::Weapon: break;
        case EItemType::Food: break;
        case EItemType::Consumable: 
            ItemType->SetText(FText::FromString("Consumable"));
            DamageValue->SetVisibility(ESlateVisibility::Collapsed);
            ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
            SellValue->SetVisibility(ESlateVisibility::Collapsed);
            break; 
            default: ;
    }
    ItemName->SetText(ItemBeingHovered->TextData.Name);
    DamageValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.DamageValue));
    ArmorRating->SetText(FText::AsNumber(ItemBeingHovered->Statistics.ArmorRating));
    UsageText->SetText(ItemBeingHovered->TextData.UsageText);
    ItemDescription->SetText(ItemBeingHovered->TextData.Description);
    SellValue->SetText(FText::AsNumber(ItemBeingHovered->Statistics.SellValue));
    StackWeight->SetText(FText::AsNumber(ItemBeingHovered->GetItemStackWeight()));
    if (ItemBeingHovered->NumericData.bIsStackble)
    {
            MaxStackSize->SetText(FText::AsNumber(ItemBeingHovered->NumericData.MaxStackSize));
    }
    else
    {
            MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
    }

}