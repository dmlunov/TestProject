// Test Game,  All Rights Reserved.
// ProjectGame
#include "Player/ProjectBaseCharacter.h"
#include "Components/TestCharacterMovementComponent.h"
#include "Components/HelthComponent.h"
#include "Components/TestWeaponComponent.h"
#include "Components/TestItemComponent.h"
#include "Components/TestInventoryComponent.h"
#include "UI/TestGameHUD.h"
// #include "ProjectCoreTypes.h"

// Engine
//#include "Camera/CameraComponent.h"
//#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"

// GAS
#include "Abilities/PGAttributeSet.h"
#include "Components/PGAbilitySystemComponent.h"
#include "Abilities/PGGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"
// #include "AbilitySystemComponent.h"
// #include "Abilities/GameplayAbility.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
AProjectBaseCharacter::AProjectBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UTestCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    /*
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(-90.0f, -100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);*/

    WeaponComponent = CreateDefaultSubobject<UTestWeaponComponent>("WeaponComponent");

    HelthComponent = CreateDefaultSubobject<UHelthComponent>("HelthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    ItemComponent = CreateDefaultSubobject<UTestItemComponent>("ItemComponent");
    InventoryComponent = CreateDefaultSubobject<UTestInventoryComponent>("InventoryComponent");
    InventoryComponent->SetSlotsCapacity(20);
    InventoryComponent->SetWeightCapacity(50.0f);

    AbilitySystemComponent = CreateDefaultSubobject<UPGAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    //*********************** GAS

   // bAbilitiesInitialized = false;
    Attributes = CreateDefaultSubobject<UPGAttributeSet>("Attributes");
   // GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);
    bAlwaysRelevant = true;

    DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
    EffectRemoveOnDeathTag = FGameplayTag::RequestGameplayTag(FName("Effect.RemoveOnDeath"));
};

// Called when the game starts or when spawned
void AProjectBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HelthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    OnHealthChanged(HelthComponent->GetHealth());
    HelthComponent->OnDeath.AddUObject(this, &AProjectBaseCharacter::OnDeath);
    HelthComponent->OnHealthChanged.AddUObject(this, &AProjectBaseCharacter::OnHealthChanged);
};

// Called every frame
void AProjectBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
};

// Called to bind functionality to input

UAbilitySystemComponent* AProjectBaseCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent.Get();
};
/*
void AProjectBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AProjectBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AProjectBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AProjectBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AProjectBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTestWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UTestWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UTestWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UTestWeaponComponent::Reload);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, ItemComponent, &UTestItemComponent::BeginInteract);
    PlayerInputComponent->BindAction("Interact", IE_Released, ItemComponent, &UTestItemComponent::EndInteract);
    PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AProjectBaseCharacter::ToggleMenu);

};*/
/*
void AProjectBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
};

void AProjectBaseCharacter::OnStartRunning()
{
    WantsToRun = true;
};
void AProjectBaseCharacter::OnStopRunning()
{
    WantsToRun = false;
};

void AProjectBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
};
void AProjectBaseCharacter::ToggleMenu()
{
    TestGameHUD->ToggleMenu();
};


*/

bool AProjectBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
};

float AProjectBaseCharacter::GetMovementDerection() const
{
    if (GetVelocity().IsZero()) return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngelBetween);

    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
};



bool AProjectBaseCharacter::IsAlive() const
{
    return GetHealth() > 0.0f;
}

int32 AProjectBaseCharacter::GetAbilityLevel(EPGAbilityInputID AbilityID) const
{
    return 1;
}

void AProjectBaseCharacter::OnDeath()
{

    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopFire();

    OnCharacterDied.Broadcast(this);
    // UE_LOG(BaseCharacterLog, Display, TEXT("Player is death, Name: %s"), *GetName());
    if (AbilitySystemComponent)
    {
        AbilitySystemComponent->CancelAllAbilities();

        FGameplayTagContainer EffectTagsToRemove;
        EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
        int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

        AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
    }

    PlayAnimMontage(DeathAnimMontage);
};

void AProjectBaseCharacter::FinishDying()
{
    Destroy();
}

void AProjectBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
};


/****************************
           GAS / AbilitySystem
*****************************/

void AProjectBaseCharacter::RemoveCharacterAbilities()
{
    if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->bCharacterAbilitiesGiven)
    {
        return;
    }
    // Remove any abilities added from a previous call. This checks to make sure the ability is in the startup 'CharacterAbilities' array.
    TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
    for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
    {
        if ((Spec.SourceObject == this) && CharacterAbilities.Contains(Spec.Ability->GetClass()))
        {
            AbilitiesToRemove.Add(Spec.Handle);
        }
    }

    // Do in two passes so the removal happens after we have the full list
    for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
    {
        AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
    }

    AbilitySystemComponent->bCharacterAbilitiesGiven = false;
}

void AProjectBaseCharacter::AddCharacterAbilities()
{
    // Grant abilities, but only on the server
    if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bCharacterAbilitiesGiven)
    {
        return;
    }

    for (TSubclassOf<UPGGameplayAbility>& StartupAbility : CharacterAbilities)
    {
        AbilitySystemComponent->GiveAbility(
            FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID),
                static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
    }

    AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}

void AProjectBaseCharacter::InitializeAttributes()
{
    if (!AbilitySystemComponent) return;

    if (!DefaultAttributes)
    {
        UE_LOG(BaseCharacterLog, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."),
            *FString(__FUNCTION__), *GetName());
        return;
    }

    // Can run on Server and Client
    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, GetCharacterLevel(), EffectContext);
    if (NewHandle.IsValid())
    {
        FActiveGameplayEffectHandle ActiveGEHandle =
            AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
    }
}

void AProjectBaseCharacter::AddStartupEffects()
{
    if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->bStartupEffectsApplied)
    {
        return;
    }

    FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
    EffectContext.AddSourceObject(this);

    for (TSubclassOf<UGameplayEffect> GameplayEffect : StartupEffects)
    {
        FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, GetCharacterLevel(), EffectContext);
        if (NewHandle.IsValid())
        {
            FActiveGameplayEffectHandle ActiveGEHandle =
                AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
        }
    }

    AbilitySystemComponent->bStartupEffectsApplied = true;
}

int32 AProjectBaseCharacter::GetCharacterLevel() const
{
    if (Attributes)
    {
        return static_cast<int32>(Attributes->GetCharacterLevel());
    }

    return 0;
}

float AProjectBaseCharacter::GetHealth() const
{
    return Attributes ? Attributes->GetHealth() : HelthComponent->GetHealth();
};

float AProjectBaseCharacter::GetMaxHealth() const
{
    return Attributes ? Attributes->GetMaxHealth() : HelthComponent->GetMaxHealth();
};

float AProjectBaseCharacter::GetMana() const
{
    return Attributes ? Attributes->GetMana() : 0.0f;
};

float AProjectBaseCharacter::GetMaxMana() const
{
    return Attributes ? Attributes->GetMaxMana() : 0.0f;
};

float AProjectBaseCharacter::GetStamina() const
{
    return Attributes ? Attributes->GetStamina() : 0.0f;
};

float AProjectBaseCharacter::GetMaxStamina() const
{
    return Attributes ? Attributes->GetMaxStamina() : 0.0f;
};

void AProjectBaseCharacter::SetHealth(float Health)
{
    if (Attributes)
    {
        // UE_LOG(HealthComponentLog, Display, TEXT("Attributs Health get helth = %f"), Health);
        Attributes->SetHealth(Health);
    }
    HelthComponent->SetHealth(Health);
};
void AProjectBaseCharacter::SetMana(float Mana)
{
    if (Attributes) Attributes->SetMana(Mana);
};
void AProjectBaseCharacter::SetStamina(float Stamina)
{
    if (Attributes) Attributes->SetStamina(Stamina);
};