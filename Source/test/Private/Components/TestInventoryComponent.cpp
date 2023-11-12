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

FItemAddResult UTestInventoryComponent::HandleNonStackableItems(UItemBase* InputItem)
{
    if (FMath::IsNearlyZero(InputItem->GetItemSingleWeight()) || InputItem->GetItemSingleWeight() < 0)
    {
        return FItemAddResult::AddedNone(  //
            FText::Format(
                FText::FromString("Counld not add {0} to the inventory. Item has invalid weight value"), InputItem->TextData.Name));
    }
    if (InventoryTotalWeight + InputItem->GetItemSingleWeight() > GetWeightCapacity())
    {
        return FItemAddResult::AddedNone(FText::Format(  //
            FText::FromString("Counld not add {0} to the inventory. Item would overfloe weight limit"), InputItem->TextData.Name));
    }
    if (InventoryContents.Num() + 1 > InventorySloatCapacity)
    {
        return FItemAddResult::AddedNone(FText::Format(  //
            FText::FromString("Counld not add {0} to the inventory. All inventery slots are full"), InputItem->TextData.Name));
    }
    AddNewItem(InputItem, 1);
    return FItemAddResult::AddedAll(1,  //
        FText::Format(FText::FromString("HandleNonStackableItems Successfully add 1 {0} to the inventory"), InputItem->TextData.Name));
}

int32 UTestInventoryComponent::HandleStackableItems(UItemBase* InputItem, int32 RequestedAddAmount)
{
    //
    return 0;
}

FItemAddResult UTestInventoryComponent::HandleAddItem(UItemBase* InputItem)
{
    if (GetOwner())
    {
        const int32 InitialRequestedAddAmount = InputItem->Quantity;
        if (InputItem->NumericData.bIsStackble)
        {
            return HandleNonStackableItems(InputItem);
        }
        const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

        if (StackableAmountAdded == InitialRequestedAddAmount)
        {
            return FItemAddResult::AddedAll(InitialRequestedAddAmount,
                FText::Format(FText::FromString("HandleAddItem Successfully add {0} {1} to the inventory"),  //
                    InitialRequestedAddAmount,                                                 //
                    InputItem->TextData.Name));
        }
        if (StackableAmountAdded < InitialRequestedAddAmount && StackableAmountAdded > 0)
        {
            return FItemAddResult::AddedPartial(StackableAmountAdded,
                FText::Format(FText::FromString("Partial amount of {0} added to inventory. Number added = {1}"),  //
                    InputItem->TextData.Name,                                                                     //
                    StackableAmountAdded));
        }
        if (StackableAmountAdded <= 0)
        {
            return FItemAddResult::AddedNone(
                FText::Format(FText::FromString("Coulddn`t add {0} to the inventory. No remaining inventory slot? or invalid item"),  //
                    InputItem->TextData.Name));
        }
    }
    check(false);
    return FItemAddResult::AddedNone(FText::FromString("TryAddItem fallthrough error. GetOwner() check somehow failed"));
}

void UTestInventoryComponent::AddNewItem(UItemBase* Item, const int32 AmountToAdd)
{
    UItemBase* NewItem;
    if (Item->bIsCopy || Item->bIsPickup)
    {
        // проверка элемента не является ли он копией
        NewItem = Item;
        NewItem->ResetItemFlags();
    }
    else
    {
        // используется когда разделяем стопку элементов или  забираем из другого инвентаря
        NewItem = Item->CreateItemCopy();
    }
    NewItem->OwningInventoryComponent = this;
    NewItem->SetQuantity(AmountToAdd);

    InventoryContents.Add(NewItem);
    InventoryTotalWeight += NewItem->GetItemStackWeight();
    OnInventoryUpdate.Broadcast();
}