// Test Game,  All Rights Reserved.

#include "UI/TestPlayerHUDWidget.h"
#include "Components/HelthComponent.h"
#include "Components/TestWeaponComponent.h"
#include "ProjectUtils.h"


float UTestPlayerHUDWidget::GetHealthPercent() const
{
    const auto HealthComponent = ProjectUtils::GetProjectPlayerComponent<UHelthComponent>(GetOwningPlayerPawn());
    if (!HealthComponent) return 0.0f;
    return HealthComponent->GetHealthPercent();
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

