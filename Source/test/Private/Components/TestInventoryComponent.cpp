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

int32 UTestInventoryComponent::CalculateWeightAddAmount(UItemBase* ItemIn, int32 RequestedAddAmount)
{
    const int32 WeightMaxAddAmount = FMath::FloorToInt((GetWeightCapacity() - InventoryTotalWeight) / ItemIn->GetItemSingleWeight());
    if (WeightMaxAddAmount >= RequestedAddAmount)
    {
        return RequestedAddAmount;
    }
    return WeightMaxAddAmount;

}
int32 UTestInventoryComponent::CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount)
{
    const int32 AddAmountToMakeFullStack = StackableItem->NumericData.MaxStackSize - StackableItem->Quantity;

    return FMath::Min(InitialRequestedAddAmount, AddAmountToMakeFullStack);
}

void UTestInventoryComponent::RemoveSingleInstanceOfItem(UItemBase* ItemToRemove)
{
    InventoryContents.RemoveSingle(ItemToRemove);

    OnInventoryUpdate.Broadcast();
}

int32 UTestInventoryComponent::RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove)
{
    const int32 ActualAmouuntToRemove = FMath::Min(DesiredAmountToRemove, ItemIn->Quantity);
    ItemIn->SetQuantity(ItemIn->Quantity - ActualAmouuntToRemove);
    InventoryTotalWeight -= ActualAmouuntToRemove * ItemIn->GetItemSingleWeight();
    OnInventoryUpdate.Broadcast();
    return ActualAmouuntToRemove;
}

void UTestInventoryComponent::SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit)
{
    if (!(InventoryContents.Num() + 1 > InventorySloatCapacity))
    {
        RemoveAmountOfItem(ItemIn, AmountToSplit);
        AddNewItem(ItemIn, AmountToSplit);
    }
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