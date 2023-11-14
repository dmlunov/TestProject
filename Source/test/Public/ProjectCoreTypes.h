#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ProjectCoreTypes.generated.h"

// weapon
DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 Bullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!Infinite"))
    int32 Clips;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool Infinite;
};

// weapon component

class ATestBaseWeapon;

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ATestBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UAnimMontage* ReloadAnimMontage;
};

// HelthComponent
DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float);

// Item Data
UENUM()
enum class EItemType : uint8
{
    Armor UMETA(DisplayName = "Armor"),
    Weapon UMETA(DisplayName = "Weapon"),
    Food UMETA(DisplayName = "Food"),
    Consumable UMETA(DisplayName = "Consumable"),
};

USTRUCT()
struct FItemStatistics
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    float ArmorRating;
    UPROPERTY(EditAnywhere)
    float DamageValue;
    UPROPERTY(EditAnywhere)
    float SellValue;
    UPROPERTY(EditAnywhere)
    float HealthValue;
};

USTRUCT()
struct FItemTextData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    FText Name;
    UPROPERTY(EditAnywhere)
    FText Description;
    UPROPERTY(EditAnywhere)
    FText InteractionText;
    UPROPERTY(EditAnywhere)
    FText UsageText;
};

USTRUCT() struct FItemNumericData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    float Weight;
    UPROPERTY(EditAnywhere)
    int MaxStackSize;
    UPROPERTY(EditAnywhere)
    bool bIsStackble;
};

USTRUCT() struct FItemAssetData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    UTexture2D* Icon;
    UPROPERTY(EditAnywhere)
    UStaticMesh* StaticMesh;
};

USTRUCT() struct FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FName ID;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    EItemType ItemType;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemStatistics ItemStatistics;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemTextData ItemTextData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemNumericData ItemNumericData;

    UPROPERTY(EditAnywhere, Category = "Item Data")
    FItemAssetData ItemAssetData;

    //UPROPERTY(EditAnywhere, Category = "Item Data")
    //FTransform Transform;
    //UPROPERTY(EditAnywhere, Category = "Item Data")
    //bool IsCanChangeTrancform{false};

};