// Test Game,  All Rights Reserved.

#include "Items/ItemBase.h"
#include "Player/TestBaseCharacter.h"
#include "Components/TestInventoryComponent.h"

UItemBase::UItemBase() : bIsCopy(false), bIsPickup(false)
{
    //
}

UItemBase* UItemBase::CreateItemCopy() const
{
    UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());

    ItemCopy->ID = this->ID;
    ItemCopy->Quantity = this->Quantity;
    ItemCopy->ItemType = this->ItemType;
    ItemCopy->Statistics = this->Statistics;
    ItemCopy->TextData = this->TextData;
    ItemCopy->NumericData = this->NumericData;
    ItemCopy->AssetData = this->AssetData;
    ItemCopy->bIsCopy = true;

    return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
    if (NewQuantity != Quantity)
    {
        Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackble ? NumericData.MaxStackSize : 1);
        if (OwningInventoryComponent)
          {
              if (Quantity <= 0)
               {
                  OwningInventoryComponent->RemoveSingleInstanceOfItem(this);
               }
          }
    }
}

void UItemBase::Use(ATestBaseCharacter* Character)
{
    //
}

void UItemBase::ResetItemFlags()
{
    bIsCopy = false;
    bIsPickup=false;
}