// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCoreTypes.h"
#include "ItemBase.generated.h"

class ATestBaseCharacter;
class UTestInventoryComponent;

UCLASS()
class TEST_API UItemBase : public UObject
{
    GENERATED_BODY()

public:
    // PROPERTY

      UPROPERTY()
    UTestInventoryComponent* OwningInventoryComponent;

    UPROPERTY(VisibleAnywhere, Category = "Item")//, meta = (UIMin = 1, UIMax = 100))
    int32 Quantity;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FName ID;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    EItemType ItemType;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FItemStatistics Statistics;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FItemTextData TextData;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FItemNumericData NumericData;

    UPROPERTY(VisibleAnywhere, Category = "Item")
    FItemAssetData AssetData;

    bool bIsCopy;
    bool bIsPickup;
    void ResetItemFlags();

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
    bool operator==(const FName& OtherID) const { return this->ID == OtherID; };
};
