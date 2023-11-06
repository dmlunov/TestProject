// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCoreTypes.h"
#include "ItemBase.generated.h"

class ATestBaseCharacter;
// class UInventoryComponent;

UCLASS()
class TEST_API UItemBase : public UObject
{
    GENERATED_BODY()

public:
    // PROPERTY

    //  UPROPERTY()
    // UInventoryComponent* InventoryComponent;

    UPROPERTY(EditAnywhere, Category = "Item Data", meta = (UIMin = 1, UIMax = 100))
    int32 Quantity;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FName ID;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    EItenType Type;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemStatistics Statistics;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemTextData TextData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemNumericData NumericData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemAssetData AssetData;

    // FUNCTION
    UItemBase();

    UFUNCTION(Category = "Item")
    UItemBase* CreateItemCopy() const;
    UFUNCTION(Category = "Item")
    FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };

    UFUNCTION(Category = "Item")
    FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };

    UFUNCTION(Category = "Item")
    FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStackSize; };

    UFUNCTION(Category = "Item")
    void SetQuantity(const int32 NewQuantity);

    UFUNCTION(Category = "Item")
    virtual void Use(ATestBaseCharacter* Character);

protected:
    // перегружаем оператор == для сравнения объектов инвентаря по ID
    bool operator==(const FName& OtherID) const { return ID == OtherID; };
};
