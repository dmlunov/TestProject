// Test Game,  All Rights Reserved.

#include "Components/HelthComponent.h"
#include "Player/ProjectBaseCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/ProjectPlayerCharacter.h"
#include "Abilities/PGAttributeSet.h"
#include "AI/ProjectAICharacter.h"
#include "TestGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(HealthComponentLog, All, All)

UHelthComponent::UHelthComponent()
{

    PrimaryComponentTick.bCanEverTick = true;
}

void UHelthComponent::BeginPlay()
{
    Super::BeginPlay();
    Character = Cast<AProjectBaseCharacter>(GetOwner());
    // auto CharacterAI = Cast<AProjectAICharacter>(GetOwner());
    // Character = Cast<AProjectPlayerCharacter>(GetOwner());
    if (Character)
    {
        if (Character->GetMaxHealth() && Character->GetHealth())
        {
            MaxHealth = Character->GetMaxHealth();
            Health = Character->GetHealth();
        }
    }

    check(MaxHealth > 0);

    SetHealth(MaxHealth);
    // if (Character) UE_LOG(HealthComponentLog, Display, TEXT("Begin Play Set Health = %f %s"), MaxHealth, *(Character->GetName()));
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
    auto CurentCharacter = Cast<AProjectBaseCharacter>(GetOwner());
    if (CurentCharacter)
    {
        CurentCharacter->GetAttributes()->SetHealth(Health - Damage);
        // UE_LOG(HealthComponentLog, Display, TEXT("Make Damage = %f"), Health - Damage);
    }

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
    if (Character)
    {
        MaxHealth = Character->GetMaxHealth();
        Health = Character->GetHealth();

        if (IsDead())
        {
            GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
            OnDeath.Broadcast();
            return;
        }
        Character->GetAttributes()->SetHealth(Health + HealModifier);
    }
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
    }
}

void UHelthComponent::SetHealth(float NewHealth)
{
    if (Character)
    {
        MaxHealth = Character->GetMaxHealth();
        // Health = Character->GetHealth();
        //  Character->GetAttributes()->SetHealth(Health + HealModifier);
    }
    const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const auto HealthDelta = NextHealth - Health;
    Health = NextHealth;

    OnHealthChanged.Broadcast(Health, HealthDelta);

    GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHelthComponent::HealUpdate, HealUpdateTime, true, HealDelay);
    }
    // if (Character) UE_LOG(HealthComponentLog, Display, TEXT("Set Health = %f %s"), Health, *Character->GetName());
    // UE_LOG(HealthComponentLog, Display, TEXT("HealthComponent set Attributes Helth = %f"), Health);
}

void UHelthComponent::Killed(AController* KillerController)
{
    if (!GetWorld()) return;
    const auto GameMode = Cast<ATestGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!GameMode) return;

    const auto Player = Cast<APawn>(GetOwner());
    const auto VikimController = Player ? Player->Controller : nullptr;
    GameMode->Killed(KillerController, VikimController);
}
