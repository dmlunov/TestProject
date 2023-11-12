// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestInventoryComponent.generated.h"

class UItemBase;

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdateSignature);

UENUM(BlueprintType)
enum class EItemAssResult : uint8
{
    IAR_NoItemAdded UMETA(DisplayName = "No Item Added"),
    IAR_PatialAmountItemAdded UMETA(DisplayName = "Patial Amount Item Added"),
    IAR_AllItemAdded UMETA(DisplayName = "All Item Added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
    GENERATED_BODY()

    FItemAddResult()
        :                                                    //
          ActualAmountAdded(0),                              //
          OperationResult(EItemAssResult::IAR_NoItemAdded),  //
          ResultMessage(FText::GetEmpty())                   //
          {};

    // Фактическое количество товара, которое было добавлено в инвентарь
    UPROPERTY(BlueprintReadOnly, Category = "Item Add Resuld")
    int32 ActualAmountAdded;
    // Enum показывающий конечное состояние инвентаря после операции добавления
    UPROPERTY(BlueprintReadOnly, Category = "Item Add Resuld")
    EItemAssResult OperationResult;
    // Информационное сообщение, которое может быть передано
    UPROPERTY(BlueprintReadOnly, Category = "Item Add Resuld")
    FText ResultMessage;

    static FItemAddResult AddedNone(const FText& ErrorText)
    {
        //
        FItemAddResult AddedNoneResult;
        AddedNoneResult.ActualAmountAdded = 0;
        AddedNoneResult.OperationResult = EItemAssResult::IAR_NoItemAdded;
        AddedNoneResult.ResultMessage = ErrorText;
        return AddedNoneResult;
    };

    static FItemAddResult AddedPartial(const int32 PartialAmounntAdded, const FText& ErrorText)
    {
        FItemAddResult AddedPartialResult;
        AddedPartialResult.ActualAmountAdded = PartialAmounntAdded;
        AddedPartialResult.OperationResult = EItemAssResult::IAR_PatialAmountItemAdded;
        AddedPartialResult.ResultMessage = ErrorText;
        return AddedPartialResult;
    };
    static FItemAddResult AddedAll(const int32 AmounntAdded, const FText& Message)
    {
        FItemAddResult AddedAllResult;
        AddedAllResult.ActualAmountAdded = AmounntAdded;
        AddedAllResult.OperationResult = EItemAssResult::IAR_AllItemAdded;
        AddedAllResult.ResultMessage = Message;
        return AddedAllResult;
    };
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEST_API UTestInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:

    FOnInventoryUpdateSignature OnInventoryUpdate;

    UTestInventoryComponent();
    UFUNCTION(Category = "Inventory")
    FItemAddResult HandleAddItem(UItemBase* InputItem);
    UFUNCTION(Category = "Inventory")
    UItemBase* FindMatchingItem(UItemBase* ItemIn) const;
    UFUNCTION(Category = "Inventory")
    UItemBase* FindNextItemByID(UItemBase* ItemIn) const;
    UFUNCTION(Category = "Inventory")
    UItemBase* FindNextPatialStack(UItemBase* ItemIn) const;

    UFUNCTION(Category = "Inventory")
    void RemoveSingleInstanceOfItem(UItemBase* ItemToRemove);
    UFUNCTION(Category = "Inventory")
    int32 RemoveAmountOfItem(UItemBase* ItemIn, int32 DesiredAmountToRemove);
    UFUNCTION(Category = "Inventory")
    void SplitExistingStack(UItemBase* ItemIn, const int32 AmountToSplit);

    UFUNCTION(Category = "Inventory")
    FORCEINLINE float GetInventoryTotalWeight() const { return InventoryTotalWeight; };
    UFUNCTION(Category = "Inventory")
    FORCEINLINE float GetWeightCapacity() const { return InventoryWeightCapacity; };
    UFUNCTION(Category = "Inventory")
    FORCEINLINE int32 GetSloatCapacity() const { return InventorySloatCapacity; };
    UFUNCTION(Category = "Inventory")
    FORCEINLINE TArray<UItemBase*> GetInventoryContents() const { return InventoryContents; };

    UFUNCTION(Category = "Inventory")
    FORCEINLINE void SetSlotsCapacity(const int32 NewSloatCapacity) { InventorySloatCapacity = NewSloatCapacity; };
    UFUNCTION(Category = "Inventory")
    FORCEINLINE void SetWeightCapacity(const float NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };

protected:
    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    float InventoryTotalWeight;
    UPROPERTY(EditAnywhere, Category = "Inventory")
    float InventoryWeightCapacity;
    UPROPERTY(EditAnywhere, Category = "Inventory")
    int32 InventorySloatCapacity;

    UPROPERTY(VisibleAnywhere, Category = "Inventory")
    TArray<TObjectPtr<UItemBase>> InventoryContents;


    FItemAddResult HandleNonStackableItems(UItemBase* ItenIn);
    int32 HandleStackableItems(UItemBase* ItenIn, int32 RequestedAddAmount);
    int32 CalculateWeightAddAmount(UItemBase* ItenIn, int32 RequestedAddAmount);
    int32 CalculateNumberForFullStack(UItemBase* StackableItem, int32 InitialRequestedAddAmount);

    void AddNewItem(UItemBase* Item, const int32 AmountToAdd);

    virtual void BeginPlay() override;

public:
};
