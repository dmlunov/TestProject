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
        case EItemType::Food:
            ItemType->SetText(FText::FromString("Food"));
            DamageValue->SetVisibility(ESlateVisibility::Collapsed);
            ArmorRating->SetVisibility(ESlateVisibility::Collapsed);
            SellValue->SetVisibility(ESlateVisibility::Collapsed);
            break;
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
    

    // const FString WeightInfo = FText::Format(NSLOCTEXT("InventoryTooltip", "WeightInfo", "Вес: {0}"), ItemBeingHovered->GetItemStackWeight()).ToString();
   const FString WeightInfo = {"" + FString::SanitizeFloat(ItemBeingHovered->GetItemStackWeight())};
    StackWeight->SetText(FText::FromString(WeightInfo));
    if (ItemBeingHovered->NumericData.bIsStackble)
    { //Макс.вместимость
            const FString StackInfo = {"" + FString::FromInt(ItemBeingHovered->NumericData.MaxStackSize)};
            MaxStackSize->SetText(FText::FromString(StackInfo));
    }
    else
    {
            MaxStackSize->SetVisibility(ESlateVisibility::Collapsed);
    }

}