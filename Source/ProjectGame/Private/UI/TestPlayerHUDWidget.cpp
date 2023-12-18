// Test Game,  All Rights Reserved.

#include "UI/TestPlayerHUDWidget.h"
#include "Components/HelthComponent.h"
#include "Components/TestWeaponComponent.h"
#include "ProjectUtils.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Player/ProjectBaseCharacter.h"
#include "Components/TextBlock.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerHUDWidgetLog, All, All);

void UTestPlayerHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    Character = Cast<AProjectBaseCharacter>(GetOwningPlayerPawn());
    Character;
    if (Character)
    {
        HealthText->SetText(FText::AsNumber(Character->GetHealth()));
        ManaText->SetText(FText::AsNumber(Character->GetMana()));
        StaminaText->SetText(FText::AsNumber(Character->GetStamina()));
    }
}

float UTestPlayerHUDWidget::GetHealthPercent() const
{
    if (!Character) return 0.0f;
    if (Character->GetMaxHealth() > 0)
        return Character->GetHealth() / Character->GetMaxHealth();
    else
        return 0.0f;

  //  const auto HealthComponent = ProjectUtils::GetProjectPlayerComponent<UHelthComponent>(GetOwningPlayerPawn());
  //  if (!HealthComponent) return 0.0f;
    //UE_LOG(PlayerHUDWidgetLog, Display, TEXT("Health Percent = %f"), HealthComponent->GetHealthPercent());

  //  return HealthComponent->GetHealthPercent();
}

float UTestPlayerHUDWidget::GetManaPercent() const
{
    if (!Character) return 0.0f;
    if (Character->GetMaxMana() > 0)
        return Character->GetMana() / Character->GetMaxMana();
    else
        return 0.0f;
}

float UTestPlayerHUDWidget::GetStaminaPercent() const
{
    if (!Character) return 0.0f;
    if (Character->GetMaxStamina() > 0)
        return Character->GetStamina() / Character->GetMaxStamina();
    else
        return 0.0f;
}

void UTestPlayerHUDWidget::SetHealth(float Health)
{
    auto THealth = FMath::RoundToInt(Health > Character->GetMaxHealth() ? Character->GetMaxHealth() : Health);

    HealthText->SetText(FText::AsNumber(THealth));
}
void UTestPlayerHUDWidget::SetMana(float Mana)
{
    auto TMana = FMath::RoundToInt(Mana > Character->GetMaxMana() ? Character->GetMaxMana() : Mana);
    ManaText->SetText(FText::AsNumber(FMath::RoundToInt(Mana)));
}
void UTestPlayerHUDWidget::SetStamina(float Stamina)
{
    auto TStamina = FMath::RoundToInt(Stamina > Character->GetMaxStamina() ? Character->GetMaxStamina() : Stamina);
    StaminaText->SetText(FText::AsNumber(TStamina));
}

bool UTestPlayerHUDWidget::GetCurrentWeaponUIDate(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = ProjectUtils::GetProjectPlayerComponent<UTestWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;
    return WeaponComponent->GetCurrentWeaponUIDate(UIData);
}

bool UTestPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = ProjectUtils::GetProjectPlayerComponent<UTestWeaponComponent>(GetOwningPlayerPawn());
    if (!WeaponComponent) return false;
    return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

bool UTestPlayerHUDWidget::IsPlayerAlive() const
{
    const auto HealthComponent = ProjectUtils::GetProjectPlayerComponent<UHelthComponent>(GetOwningPlayerPawn());
    return HealthComponent && !HealthComponent->IsDead();
}

bool UTestPlayerHUDWidget::IsPlayerSpectating() const
{
    const auto Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

/*
UTestWeaponComponent* UTestPlayerHUDWidget::GetWeaponComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;

    const auto Component = Player->GetComponentByClass(UTestWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<UTestWeaponComponent>(Component);
    return WeaponComponent;
}

UHelthComponent* UTestPlayerHUDWidget::GetHealthComponent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;
    const auto Component = Player->GetComponentByClass(UHelthComponent::StaticClass());
    const auto HealthComponent = Cast<UHelthComponent>(Component);
    return HealthComponent;
}
*/
