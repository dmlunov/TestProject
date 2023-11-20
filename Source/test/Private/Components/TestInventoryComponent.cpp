// Test Game,  All Rights Reserved.

#include "Components/TestInventoryComponent.h"
#include "Items/ItemBase.h"
#include "Player/TestBaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(InventoryComponentLog, All, All);

UTestInventoryComponent::UTestInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTestInventoryComponent::BeginPlay()
{
    Super::BeginPlay();

    Character = Cast<ATestBaseCharacter>(GetOwner());
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

    if (RequestedAddAmount <= 0 || FMath::IsNearlyZero(InputItem->GetItemStackWeight()))
    {
        return 0;
    }
    int32 AmountToDistribute = RequestedAddAmount;
    // ��������� ��������� �� ����� � ��������� � �� �������� �� �� ���������
    UItemBase* ExistingItemStack = FindNextPatialStack(InputItem);
    // ������� ��������� ���� � ���� ����
    while (ExistingItemStack)
    {
        // �������  ��������� ����������� ��� �������� ������� ����
        const int32 AmountToMakeFullStack = CalculateNumberForFullStack(ExistingItemStack, AmountToDistribute);
        // ����� ���� ����� ��� ������ ����������������
        const int32 WeightLimitAddAmount = CalculateWeightAddAmount(ExistingItemStack, AmountToMakeFullStack);

        // ������� ���� ��������� ����� �� �������� ����������������
        if (WeightLimitAddAmount > 0)
        {
            // ������������ ���������� ������������ ��������� � ����� ��� ���������
            ExistingItemStack->SetQuantity(ExistingItemStack->Quantity + WeightLimitAddAmount);
            InventoryTotalWeight += (ExistingItemStack->GetItemSingleWeight() * WeightLimitAddAmount);

            AmountToDistribute -= WeightLimitAddAmount;

            InputItem->SetQuantity(AmountToDistribute);

            // �� ���� ��� ������� �� ����� �� ��������
            // ���� ������������ ��������������� ���������� �� �������

            if (InventoryTotalWeight + ExistingItemStack->GetItemSingleWeight() > InventoryWeightCapacity)
            {
                OnInventoryUpdate.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }
        }
        else if (WeightLimitAddAmount <= 0)
        {
            if (AmountToDistribute != RequestedAddAmount)
            {
                // ���� ����� ������������ �� �� �������� ������� � ����� ��������� ������
                OnInventoryUpdate.Broadcast();
                return RequestedAddAmount - AmountToDistribute;
            }
            return 0;
        }
        if (AmountToDistribute <= 0)
        {
            // ��� ������������� �� ������
            OnInventoryUpdate.Broadcast();
            return RequestedAddAmount;
        }
        // �������� ���� �� ��� ����
        ExistingItemStack = FindNextPatialStack(InputItem);
    }
    // ���������, ����� �� �������� ����� ����
    if (InventoryContents.Num() + 1 <= InventorySloatCapacity)
    {
        // ��������� �������� ��� ����� ������ �� ���������� ���������
        const int32 WeightItemLimitAddAmount = CalculateWeightAddAmount(InputItem, AmountToDistribute);
        if (WeightItemLimitAddAmount > 0)
        {
            // ���� ������ ���� ���������
            if (WeightItemLimitAddAmount < AmountToDistribute)
            {
                // ��������� ����� ���� �������� ������� �����
                AmountToDistribute -= WeightItemLimitAddAmount;
                InputItem->SetQuantity(AmountToDistribute);
                // ������� ����� � ��������� , ��� �� �� ��������� ������ �� ���� ���������� �� �����
                AddNewItem(InputItem->CreateItemCopy(), WeightItemLimitAddAmount);
                return RequestedAddAmount - AmountToDistribute;
            }
            AddNewItem(InputItem, AmountToDistribute);
            return RequestedAddAmount;
        }
       // OnInventoryUpdate.Broadcast();
        return RequestedAddAmount - AmountToDistribute;
    }
    return 0;
}

FItemAddResult UTestInventoryComponent::HandleAddItem(UItemBase* InputItem)
{

    if (GetOwner())
    {

        const int32 InitialRequestedAddAmount = InputItem->Quantity;
        if (!InputItem->NumericData.bIsStackble)
        {
            return HandleNonStackableItems(InputItem);
        }
        const int32 StackableAmountAdded = HandleStackableItems(InputItem, InitialRequestedAddAmount);

        if (StackableAmountAdded == InitialRequestedAddAmount)
        {
            return FItemAddResult::AddedAll(InitialRequestedAddAmount,
                FText::Format(FText::FromString("HandleAddItem Successfully add {0} {1} to the inventory"),  //
                    InitialRequestedAddAmount,                                                               //
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
        // �������� �������� �� �������� �� �� ������
        NewItem = Item;
        NewItem->ResetItemFlags();
        // UE_LOG(InventoryComponentLog, Display, TEXT("Item %s , %s"), *NewItem->TextData.Name.ToString(),
        //    *NewItem->Transform.GetScale3D().ToString());
    }
    else
    {
        // ������������ ����� ��������� ������ ��������� ���  �������� �� ������� ���������
        NewItem = Item->CreateItemCopy();
        // UE_LOG(InventoryComponentLog, Display, TEXT("NewItem %s , Item %s"), *NewItem->Transform.GetScale3D().ToString(),
        // *Item->Transform.GetScale3D().ToString());
    }
    NewItem->OwningInventoryComponent = this;
    NewItem->SetQuantity(AmountToAdd);

    InventoryContents.Add(NewItem);
    InventoryTotalWeight += NewItem->GetItemStackWeight();
    OnInventoryUpdate.Broadcast();
}