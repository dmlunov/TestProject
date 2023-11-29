// Test Game,  All Rights Reserved.

#include "Items/ItemBase.h"
#include "Player/ProjectBaseCharacter.h"
#include "Components/TestInventoryComponent.h"
#include "ProjectCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(ItemBaseLog, All, All);

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
    ItemCopy->Transform = this->Transform;
    ItemCopy->ItemPhysicalMass = this->ItemPhysicalMass;

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
        else
        {
            UE_LOG(ItemBaseLog, Display, TEXT("Item OwningInventory was Null!"))
        }
    }
}

void UItemBase::Use(AProjectBaseCharacter* Character)
{
    //
}

void UItemBase::ResetItemFlags()
{
    bIsCopy = false;
    bIsPickup = false;
}