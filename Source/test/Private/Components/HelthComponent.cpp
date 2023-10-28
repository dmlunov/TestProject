// Test Game,  All Rights Reserved.

#include "Components/HelthComponent.h"
#include "Player/TestBaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

UHelthComponent::UHelthComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void UHelthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHelthComponent::OnTakeAnyDamage);
    }
}

void UHelthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHelthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
}

void UHelthComponent::HealUpdate()
{

    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual( Health , MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UHelthComponent::SetHealth(float NewHealth)
{

    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}