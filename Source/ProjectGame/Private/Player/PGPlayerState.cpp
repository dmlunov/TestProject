// Test Game,  All Rights Reserved.

#include "Player/PGPlayerState.h"
#include "Abilities/PGAttributeSet.h"
#include "Components/PGAbilitySystemComponent.h"
#include "Player/TestPlayerController.h"

APGPlayerState::APGPlayerState()
{
    // Create ability system component, and set it to be explicitly replicated
    AbilitySystemComponent = CreateDefaultSubobject<UPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);

    // Mixed mode means we only are replicated the GEs to ourself, not the GEs to simulated proxies. If another GDPlayerState (Hero)
    // receives a GE, we won't be told about it by the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create the attribute set, this replicates by default
    // Adding it as a subobject of the owning actor of an AbilitySystemComponent
    // automatically registers the AttributeSet with the AbilitySystemComponent
    AttributeSetBase = CreateDefaultSubobject<UPGAttributeSet>(TEXT("AttributeSetBase"));

    // Set PlayerState's NetUpdateFrequency to the same as the Character.
    // Default is very low for PlayerStates and introduces perceived lag in the ability system.
    // 100 is probably way too high for a shipping game, you can adjust to fit your needs.
    NetUpdateFrequency = 100.0f;

    // Cache tags
    DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent* APGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

bool APGPlayerState::IsAlive() const
{
    return GetHealth() > 0.0f;
}

void APGPlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        0;
        // UTestPlayerHUDWidget* HUD = PC->GetHUD();
        //  if (HUD)  {HUD->ShowAbilityConfirmCancelText(ShowText);  }
    }
}

UPGAttributeSet* APGPlayerState::GetAttributeSetBase() const
{
    return AttributeSetBase;
}


float APGPlayerState::GetHealth() const
{  //////////////////////////////
    return AttributeSetBase->GetHealth();
}
float APGPlayerState::GetMaxHealth() const
{  //////////////////////////////
    return AttributeSetBase->GetMaxHealth();
}
float APGPlayerState::GetHealthRegenRate() const
{  //////////////////////////////
    return AttributeSetBase->GetHealthRegenRate();
}
float APGPlayerState::GetMana() const
{  //////////////////////////////
    return AttributeSetBase->GetMana();
}
float APGPlayerState::GetMaxMana() const
{  //////////////////////////////
    return AttributeSetBase->GetMaxMana();
}
float APGPlayerState::GetManaRegenRate() const
{  //////////////////////////////
    return AttributeSetBase->GetManaRegenRate();
}
float APGPlayerState::GetStamina() const
{  //////////////////////////////
    return AttributeSetBase->GetStamina();
}
float APGPlayerState::GetMaxStamina() const
{  //////////////////////////////
    return AttributeSetBase->GetMaxStamina();
}
float APGPlayerState::GetStaminaRegenRate() const
{  //////////////////////////////
    return AttributeSetBase->GetStaminaRegenRate();
}

float APGPlayerState::GetArmor() const
{  //////////////////////////////
    return AttributeSetBase->GetArmor();
}

float APGPlayerState::GetMoveSpeed() const
{  //////////////////////////////
    return AttributeSetBase->GetMoveSpeed();
}

int32 APGPlayerState::GetCharacterLevel() const
{  //////////////////////////////
    return AttributeSetBase->GetCharacterLevel();
}

int32 APGPlayerState::GetXP() const
{  //////////////////////////////
    return AttributeSetBase->GetXP();
}

int32 APGPlayerState::GetXPBounty() const
{  //////////////////////////////
    return AttributeSetBase->GetXPBounty();
}

int32 APGPlayerState::GetGold() const
{  //////////////////////////////
    return AttributeSetBase->GetGold();
}

int32 APGPlayerState::GetGoldBounty() const
{  //////////////////////////////
    return AttributeSetBase->GetGoldBounty();
}

void APGPlayerState::BeginPlay()
{
    Super::BeginPlay();

    if (AbilitySystemComponent)
    {
        // Attribute change callbacks
         HealthChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute())
                .AddUObject(this, &APGPlayerState::HealthChanged);
        MaxHealthChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute())
                .AddUObject(this, &APGPlayerState::MaxHealthChanged);
        HealthRegenRateChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute())
                .AddUObject(this, &APGPlayerState::HealthRegenRateChanged);
        ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaAttribute())
                                        .AddUObject(this, &APGPlayerState::ManaChanged);
        MaxManaChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxManaAttribute())
                .AddUObject(this, &APGPlayerState::MaxManaChanged);
        ManaRegenRateChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetManaRegenRateAttribute())
                .AddUObject(this, &APGPlayerState::ManaRegenRateChanged);
        StaminaChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute())
                .AddUObject(this, &APGPlayerState::StaminaChanged);
        MaxStaminaChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute())
                .AddUObject(this, &APGPlayerState::MaxStaminaChanged);
        StaminaRegenRateChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute())
                .AddUObject(this, &APGPlayerState::StaminaRegenRateChanged);
         XPChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetXPAttribute())
                                      .AddUObject(this, &APGPlayerState::XPChanged);
        GoldChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetGoldAttribute())
                                        .AddUObject(this, &APGPlayerState::GoldChanged);
        CharacterLevelChangedDelegateHandle =
            AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetCharacterLevelAttribute())
                .AddUObject(this, &APGPlayerState::CharacterLevelChanged);

        // Tag change callbacks
        AbilitySystemComponent
            ->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved)
            .AddUObject(this, &APGPlayerState::StunTagChanged);
    }
}

void APGPlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
    float Health = Data.NewValue;

    // Update floating status bar
    /* AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
    if (Hero)
    {
        UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
        if (HeroFloatingStatusBar)
        {
            HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
        }
    }

    // Update the HUD
    // Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint

    // If the player died, handle death
    if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
    {
        if (Hero)
        {
            Hero->Die();
        }
    }*/
}

void APGPlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
    float MaxHealth = Data.NewValue;
    /*
    // Update floating status bar
    AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
    if (Hero)
    {
        UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
        if (HeroFloatingStatusBar)
        {
            HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
        }
    }

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetMaxHealth(MaxHealth);
        }
    }*/
}

void APGPlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
    float HealthRegenRate = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /*UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetHealthRegenRate(HealthRegenRate);
        }*/
    }
}

void APGPlayerState::ManaChanged(const FOnAttributeChangeData& Data)
{
    float Mana = Data.NewValue;

    // Update floating status bar
    /* AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
    if (Hero)
    {
        UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
        if (HeroFloatingStatusBar)
        {
            HeroFloatingStatusBar->SetManaPercentage(Mana / GetMaxMana());
        }
    }*/

    // Update the HUD
    // Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
}

void APGPlayerState::MaxManaChanged(const FOnAttributeChangeData& Data)
{
    float MaxMana = Data.NewValue;

    // Update floating status bar
    /* AGDHeroCharacter* Hero = Cast<AGDHeroCharacter>(GetPawn());
    if (Hero)
    {
        UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
        if (HeroFloatingStatusBar)
        {
            HeroFloatingStatusBar->SetManaPercentage(GetMana() / MaxMana);
        }
    }

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetMaxMana(MaxMana);
        }
    }*/
}

void APGPlayerState::ManaRegenRateChanged(const FOnAttributeChangeData& Data)
{
    float ManaRegenRate = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetManaRegenRate(ManaRegenRate);
        }*/
    }
}

void APGPlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
    float Stamina = Data.NewValue;

    // Update the HUD
    // Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
}

void APGPlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
    float MaxStamina = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetMaxStamina(MaxStamina);
        }*/
    }
}

void APGPlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
    float StaminaRegenRate = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetStaminaRegenRate(StaminaRegenRate);
        }*/
    }
}

void APGPlayerState::XPChanged(const FOnAttributeChangeData& Data)
{
    float XP = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetExperience(XP);
        }*/
    }
}

void APGPlayerState::GoldChanged(const FOnAttributeChangeData& Data)
{
    float Gold = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetGold(Gold);
        }*/
    }
}

void APGPlayerState::CharacterLevelChanged(const FOnAttributeChangeData& Data)
{
    float CharacterLevel = Data.NewValue;

    // Update the HUD
    ATestPlayerController* PC = Cast<ATestPlayerController>(GetOwner());
    if (PC)
    {
        /* UGDHUDWidget* HUD = PC->GetHUD();
        if (HUD)
        {
            HUD->SetHeroLevel(CharacterLevel);
        }*/
    }
}

void APGPlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
    if (NewCount > 0)
    {
        FGameplayTagContainer AbilityTagsToCancel;
        AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

        FGameplayTagContainer AbilityTagsToIgnore;
        AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

        AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
    }
}
