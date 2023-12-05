// Test Game,  All Rights Reserved.
#include "Items/ItemBase.h"
#include "UI/Inventory/InventoryActionMessage.h"
#include "UI/Inventory/InventoryItemSlot.h"
#include "UI/TestGameHUD.h"
#include "UI/MainMenu.h"
#include "Player/ProjectBaseCharacter.h"
#include "ProjectUtils.h"
#include "Components/TestWeaponComponent.h"
#include "Weapon/TestBaseWeapon.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(InventoryActionMessageLog, All, All);

void UInventoryActionMessage::NativeConstruct()
{
    Super::NativeConstruct();

    if (ActionButton)
    {
        ActionButton->OnClicked.AddDynamic(this, &UInventoryActionMessage::ActionItem);
    }
    if (CloseWidgetButton)
    {
        CloseWidgetButton->OnClicked.AddDynamic(this, &UInventoryActionMessage::CloseWidget);
    }
}

void UInventoryActionMessage::ChangeData()
{
    if (!InventorySlot)
    {
        UE_LOG(InventoryActionMessageLog, Display, TEXT("InventorySlot is empty"));
        return;
    }

    ItemBeingHovered = InventorySlot->GetItemReference();

    if (!ItemBeingHovered)
    {
         UE_LOG(InventoryActionMessageLog, Display, TEXT("ItemBeingHovered is empty"));
        return;
    }

    ItemType = ItemBeingHovered->ItemType;

    switch (ItemType)
    {
        case EItemType::Armor: break;
        case EItemType::Weapon:
            ItemTypeText->SetText(WeaponTypeMessage);
            ActionMessage->SetText(WeaponActionMessage);
            break;
        case EItemType::Food:
            ItemTypeText->SetText(FoodTypeMessage);
            ActionMessage->SetText(FoodActionMessage);
            break;
        case EItemType::Consumable:
            ItemTypeText->SetText(FText::FromString("Consumable"));
            ActionMessage->SetVisibility(ESlateVisibility::Collapsed);
            break;
        default:;
    }

    ItemName->SetText(ItemBeingHovered->TextData.Name);
    // UE_LOG(InventoryActionMessageLog, Display, TEXT("Visiblety ActionMessage type item "),);
    SetVisibility(ESlateVisibility::Visible);
}

void UInventoryActionMessage::ActionItem()
{
    switch (ItemType)
    {
        case EItemType::Armor: break;
        case EItemType::Weapon: ActionWeapon(); break;
        case EItemType::Food: break;
        case EItemType::Consumable: break;
        default:;
    }
    //UE_LOG(InventoryActionMessageLog, Display, TEXT("Clicked Button ActionMessage"));
    CloseWidget();
}

void UInventoryActionMessage::ActionWeapon()
{
    //ItemBeingHovered = InventorySlot->GetItemReference();
    const FString WeaponName = ItemBeingHovered->TextData.Name.ToString();

    //UE_LOG(InventoryActionMessageLog, Display, TEXT("Select Weapon Name = %s"), *WeaponName);

    UTestWeaponComponent* WeaponComponent = ProjectUtils::GetProjectPlayerComponent<UTestWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return;

    TArray<ATestBaseWeapon*> WeapoinInInventary = WeaponComponent->GetWeaponsInInventary();
    ATestBaseWeapon* CurrentWeapon = WeaponComponent->GetCurrentWeapon();
    if (!CurrentWeapon) return;
    int32 count = 0;
    int32 CountWeapons = WeapoinInInventary.Num();
    while (!CurrentWeapon->GetName().Contains(WeaponName))
    {
        WeaponComponent->FastNextWeapon();
        CurrentWeapon = WeaponComponent->GetCurrentWeapon();

       // UE_LOG(InventoryActionMessageLog, Display, TEXT("Current Weapon Name = %s ,count = %i "), *CurrentWeapon->GetName(), count);
        if (count >= CountWeapons) break;
        count++;
    }
    //UE_LOG(InventoryActionMessageLog, Display, TEXT("Current Weapon Name = %s "), *CurrentWeapon->GetName());
}

void UInventoryActionMessage::CloseWidget()
{
    //UE_LOG(InventoryActionMessageLog, Display, TEXT("CloseWidget ActionMessage"));
    // RemoveFromParent();
    ATestGameHUD* TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    TestGameHUD->ToggleMenu();
    // SetVisibility(ESlateVisibility::Collapsed);
}