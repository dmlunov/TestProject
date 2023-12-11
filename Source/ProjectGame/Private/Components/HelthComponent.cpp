// Test Game,  All Rights Reserved.

#include "Components/HelthComponent.h"
#include "Player/ProjectBaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
//#include "Abilities/PGAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

UHelthComponent::UHelthComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void UHelthComponent::BeginPlay()
{
    Super::BeginPlay();

    check(MaxHealth > 0);
    SetHealth(MaxHealth);

   // AProjectBaseCharacter*  Character = Cast<AProjectBaseCharacter>(GetOwner());
   // if (Character) { Attributes = Character->GetAttributes(); }

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

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UHelthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
   // UE_LOG(HealthComponentLog, Display, TEXT("Health get = %f"), Health);
    
   // if (Attributes)
   //  { UE_LOG(HealthComponentLog, Display, TEXT("Attributs Health get helth = %f"), Health);
   //     Attributes->SetHealth(Health);}
}
