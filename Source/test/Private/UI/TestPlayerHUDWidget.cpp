// Test Game,  All Rights Reserved.


#include "UI/TestPlayerHUDWidget.h"
#include "Components/HelthComponent.h"


float UTestPlayerHUDWidget::GetHealthPercent() const
{
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;
    const auto Component = Player->GetComponentByClass(UHelthComponent::StaticClass());
    const auto HealthComponent = Cast<UHelthComponent>(Component);
    if (!HealthComponent) return 0.0f;
    return HealthComponent->GetHealthPercent();
}