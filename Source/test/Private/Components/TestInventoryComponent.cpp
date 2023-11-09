// Test Game,  All Rights Reserved.

#include "Components/TestInventoryComponent.h"
#include "Items/ItemBase.h"

UTestInventoryComponent::UTestInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTestInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

UItemBase* UTestInventoryComponent::FindMatchingItem(UItemBase* ItemIn) const
{
    if (ItemIn)
        if (InventoryContents.Contains(ItemIn))
        {
            return ItemIn;
        }
    return nullptr;
}

UItemBase* UTestInventoryComponent::FindNextItemByID(UItemBase* ItemIn) const
{
    if (ItemIn)
    {
        if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
        {
            return *Result;
        }
    }
    return nullptr;
}

UItemBase* UTestInventoryComponent::FindNextPatialStack(UItemBase* ItemIn) const
{
    if (const TArray<TObjectPtr<UItemBase>>::ElementType* Result = InventoryContents.FindByPredicate(
        [&ItemIn](const UItemBase* InventoryItem) { return InventoryItem->ID == ItemIn->ID && !InventoryItem->IsFullItemStack(); }))
    {
        return *Result;
    }
    return nullptr;
}

int32 UTestInventoryComponent::CalculateWeightAddAmount(UItemBase*, int32 RequestedAddAmount)
{
    //
    return 0;
}
int32 UTestInventoryComponent::CalculateNumberForFullStack(UItemBase* ExistingItem, int32 InitialRequestedAddAmount)
{
    //
    return 0;
}

void UTestInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemIn)
{
    //
}

int32 UTestInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
    //
    return 0;
}

void UTestInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
    //
}

FItemAddResult UTestInventoryComponent::HandleNonStackableItems(UItemBase*, int32 RequestedAddAmount)
{
    //
    FItemAddResult a;
    return a;
}
int32 UTestInventoryComponent::HandleStackableItems(UItemBase*, int32 RequestedAddAmount)
{
    //
    return RequestedAddAmount;
}

FItemAddResult UTestInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
    FItemAddResult a;
    return  a;
}

void UTestInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
    //
}