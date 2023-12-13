// Test Game,  All Rights Reserved.

#include "Components/HelthComponent.h"
#include "Player/ProjectBaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
 #include "Player/ProjectPlayerCharacter.h"
#include "Abilities/PGAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

UHelthComponent::UHelthComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void UHelthComponent::BeginPlay()
{
    Super::BeginPlay();
    
    Character = Cast<AProjectPlayerCharacter>(GetOwner());
    if (Character)
    {
        MaxHealth = Character->GetMaxHealth();
        Health = Character->GetHealth();
    }
check(MaxHealth > 0);
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

    // if (Character) Character->SetHealth(Character->GetHealth() - Damage);

    // SetHealth(Character->GetHealth());

    // else

    SetHealth(Health - Damage);

    if (Character) Character->GetAttributes()->SetHealth(Health - Damage);

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

    if (Character) Character->GetAttributes()->SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UHelthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHelthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }

    // UE_LOG(HealthComponentLog, Display, TEXT("HealthComponent set Attributes Helth = %f"), Health);
}
