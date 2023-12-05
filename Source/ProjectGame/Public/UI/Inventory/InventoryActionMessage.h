// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectCoreTypes.h"
#include "InventoryActionMessage.generated.h"

class UBorder;
class UButton;
class UInventoryItemSlot;
class UTextBlock;
class UItemBase;


UCLASS()
class PROJECTGAME_API UInventoryActionMessage : public UUserWidget
{

    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere)
    UInventoryItemSlot* InventorySlot;

    UFUNCTION()
    void CloseWidget();

    UFUNCTION()
    void ChangeData();

protected:
    UPROPERTY (VisibleAnywhere, Category = "Action Message", meta = (BindWidget))
    UBorder* ActionBorder;

    UPROPERTY (VisibleAnywhere, Category = "Action Message", meta = (BindWidget))
    UButton* ActionButton;

    UPROPERTY (VisibleAnywhere, Category = "Action Message", meta = (BindWidget))
    UButton* CloseWidgetButton;

    UPROPERTY (meta = (BindWidget))
    UTextBlock* ItemName;

    UPROPERTY (meta = (BindWidget))
    UTextBlock* ItemTypeText;

    UPROPERTY (meta = (BindWidget))
    UTextBlock* ActionMessage;

    UPROPERTY()
    UItemBase* ItemBeingHovered;

    UPROPERTY()
    EItemType ItemType;

    UFUNCTION()
    void ActionItem();

    UFUNCTION()
    void ActionWeapon();

    virtual void NativeConstruct() override;

    //  #define LOCALTEXT_NAMESPACE "ProjectGame"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Message")
    FText WeaponTypeMessage = NSLOCTEXT("Game", "InventoryActionMessageText", "Weapon");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Message")
    FText WeaponActionMessage = NSLOCTEXT("Game", "InventoryActionMessageText", "Select");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Message")
    FText FoodTypeMessage = NSLOCTEXT("Game", "InventoryActionMessageText", "Food");

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Message")
    FText FoodActionMessage = NSLOCTEXT("Game", "InventoryActionMessageText", "Eat");

    //   #undef LOCALTEXT_NAMESPACE

private:
};
