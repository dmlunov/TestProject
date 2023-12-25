// Test Game,  All Rights Reserved.

#include "Player/ProjectPlayerCharacter.h"
#include "Components/TestCharacterMovementComponent.h"
#include "Components/HelthComponent.h"
#include "Components/TestWeaponComponent.h"
#include "Components/TestItemComponent.h"
#include "Components/TestInventoryComponent.h"
#include "UI/TestGameHUD.h"
#include "Player/PGPlayerState.h"
#include "Player/TestPlayerController.h"
#include "ProjectCoreTypes.h"
#include "AI/PGAIController.h"
#include "ProjectGame/ProjectGame.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "TimerManager.h"
#include "Engine/World.h"

// GAS
#include "Abilities/PGAttributeSet.h"
#include "Components/PGAbilitySystemComponent.h"
#include "Abilities/PGGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbilityTypes.h"

DEFINE_LOG_CATEGORY_STATIC(PlayerCharacterLog, All, All);

AProjectPlayerCharacter::AProjectPlayerCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(-90.0f, -100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->FieldOfView = 80.0f;

     //GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    // GunComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Gun"));

    // Makes sure that the animations play on the Server so that we can use bone and socket transforms
    // to do things like spawning projectiles and other FX.
    //GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
   // GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
   // GetMesh()->SetCollisionProfileName(FName("NoCollision"));
    /*
            UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("UIFloatingStatusBarComponent"));
            UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
            UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0, 0, 120));
            UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
            UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500, 500));

            UIFloatingStatusBarClass = StaticLoadClass(
                UObject::StaticClass(), nullptr, TEXT("/Game/GASDocumentation/UI/UI_FloatingStatusBar_Hero.UI_FloatingStatusBar_Hero_C"));
            if (!UIFloatingStatusBarClass)
            {
                UE_LOG(LogTemp, Error,
                    TEXT("%s() Failed to find UIFloatingStatusBarClass. If it was moved, please update the reference location in C++."),
                    *FString(__FUNCTION__));
            }*/
// AIControllerClass = ATestPlayerController::StaticClass();

    DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));

}

void AProjectPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController());
    if (PlayerController)
    {
        PlayerController->CreateHUD();
    }
    // Only needed for Player placed in world and when the player is the Server.
    // On respawn, they are set up in PossessedBy.
    // When the player a client, the floating status bars are all set up in OnRep_PlayerState.
    // InitializeFloatingStatusBar();

    CameraSpringArmLength = SpringArmComponent->TargetArmLength;
    CameraSpringArmLocation = SpringArmComponent->GetRelativeLocation();
}

// Called to bind functionality to input
void AProjectPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &AProjectPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AProjectPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &AProjectPlayerCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &AProjectPlayerCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProjectPlayerCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProjectPlayerCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &AProjectPlayerCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTestWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UTestWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UTestWeaponComponent::NextWeapon);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UTestWeaponComponent::Reload);

    PlayerInputComponent->BindAction("Interact", IE_Pressed, ItemComponent, &UTestItemComponent::BeginInteract);
    PlayerInputComponent->BindAction("Interact", IE_Released, ItemComponent, &UTestItemComponent::EndInteract);
    PlayerInputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AProjectPlayerCharacter::ToggleMenu);

    /*
     PlayerInputComponent->BindAxis("Turn", this, &AGDHeroCharacter::Turn);
     PlayerInputComponent->BindAxis("TurnRate", this, &AGDHeroCharacter::TurnRate);*/

    // Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
    BindASCInput();
}

// Server only
/* void AProjectPlayerCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    APGPlayerState* Player_State = GetPlayerState<APGPlayerState>();
    if (Player_State)
    {
        // Set the ASC on the Server. Clients do this in OnRep_PlayerState()
        AbilitySystemComponent = Cast<UPGAbilitySystemComponent>(Player_State->GetAbilitySystemComponent());

        // AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have
        // PlayerControllers.
        Player_State->GetAbilitySystemComponent()->InitAbilityActorInfo(Player_State, this);

        // Set the AttributeSetBase for convenience attribute functions
        Attributes = Player_State->GetAttributeSetBase();

        // If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining
        // doesn't reset attributes. For now assume possession = spawn/respawn.
        InitializeAttributes();

        // Respawn specific things that won't affect first possession.

        // Forcibly set the DeadTag count to 0
        AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

        // Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
        SetHealth(GetMaxHealth());
        SetMana(GetMaxMana());
        SetStamina(GetMaxStamina());

        // End respawn specific things

        AddStartupEffects();

        AddCharacterAbilities();

        ATestPlayerController* PlayerController = Cast<ATestPlayerController>(GetController());
        if (PlayerController)
        {
            PlayerController->CreateHUD();
        }

        //  InitializeFloatingStatusBar();
    }
}*/

/*
UGDFloatingStatusBarWidget* AProjectPlayerCharacter::GetFloatingStatusBar()
{
    return UIFloatingStatusBar;
}

USkeletalMeshComponent* AProjectPlayerCharacter::GetGunComponent() const
{
    return GunComponent;
}

void AProjectPlayerCharacter::FinishDying()
{
    if (GetLocalRole() == ROLE_Authority)
    {
        AGASDocumentationGameMode* GM = Cast<AGASDocumentationGameMode>(GetWorld()->GetAuthGameMode());

        if (GM)
        {
            GM->HeroDied(GetController());
        }
    }

    Super::FinishDying();
}
*/
/**
 * On the Server, Possession happens before BeginPlay.
 * On the Client, BeginPlay happens before Possession.
 * So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from
 * possession.
 */

void AProjectPlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    /*
    if (GunComponent && GetMesh())
    {
        GunComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GunSocket"));
    }*/
}

/*
void AProjectPlayerCharacter::InitializeFloatingStatusBar()
{
    // Only create once
    if (UIFloatingStatusBar || !AbilitySystemComponent.IsValid())
    {
        return;
    }

    // Setup UI for Locally Owned Players only, not AI or the server's copy of the PlayerControllers
    AGDPlayerController* PC = Cast<AGDPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PC && PC->IsLocalPlayerController())
    {
        if (UIFloatingStatusBarClass)
        {
            UIFloatingStatusBar = CreateWidget<UGDFloatingStatusBarWidget>(PC, UIFloatingStatusBarClass);
            if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
            {
                UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

                // Setup the floating status bar
                UIFloatingStatusBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
                UIFloatingStatusBar->SetManaPercentage(GetMana() / GetMaxMana());
            }
        }
    }
}
*/
// Client only
void AProjectPlayerCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    APGPlayerState* PS = GetPlayerState<APGPlayerState>();
    if (PS)
    {
        // Set the ASC for clients. Server does this in PossessedBy.
        AbilitySystemComponent = Cast<UPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

        // Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
        AbilitySystemComponent->InitAbilityActorInfo(PS, this);

        // Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
        BindASCInput();

        // Set the AttributeSetBase for convenience attribute functions
        Attributes = PS->GetAttributeSetBase();

        // If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining
        // doesn't reset attributes. For now assume possession = spawn/respawn.
        InitializeAttributes();

        ATestPlayerController* PC = Cast<ATestPlayerController>(GetController());
        if (PC)
        {
            PC->CreateHUD();
        }

        // Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here
        // InitializeFloatingStatusBar();

        // Respawn specific things that won't affect first possession.

        // Forcibly set the DeadTag count to 0
        AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

        // Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
        SetHealth(GetMaxHealth());
        SetMana(GetMaxMana());
        SetStamina(GetMaxStamina());
    }
}

void AProjectPlayerCharacter::BindASCInput()
{
    if (!ASCInputBound && AbilitySystemComponent && InputComponent)
    {

        FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/ProjectGame"), FName("EPGAbilityInputID"));
        AbilitySystemComponent->BindAbilityActivationToInputComponent(
            InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), AbilityEnumAssetPath,
                                static_cast<int32>(EPGAbilityInputID::Confirm), static_cast<int32>(EPGAbilityInputID::Cancel)));
        ASCInputBound = true;
    }
}

void AProjectPlayerCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
};

void AProjectPlayerCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
};

void AProjectPlayerCharacter::OnStartRunning()
{
    if (GetStamina() <= 0.f) return;

    WantsToRun = true;

    if (IsAlive())
    {
        GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &AProjectPlayerCharacter::StaminaUpdate, 1.0f, true, 0.0f);
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
    }
};

void AProjectPlayerCharacter::OnStopRunning()
{
    WantsToRun = false;
    // auto proverka = FMath::IsNearlyEqual(GetStamina(), GetMaxStamina()) ? FString{"true"} : FString{"false"};
    // UE_LOG(PlayerCharacterLog, Display, TEXT("Stop Running Stamina = %f, MaxStamina = %f, %s"), GetStamina(), GetMaxStamina(),
    // *proverka);
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
    }
};

void AProjectPlayerCharacter::StaminaUpdate()
{
    Attributes->SetStamina(GetStamina() - 5.0f);

    if (FMath::IsNearlyEqual(GetStamina(), 0.0f) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
        WantsToRun = false;
    }
}

void AProjectPlayerCharacter::ToggleMenu()
{
    TestGameHUD->ToggleMenu();
};

/*
void AProjectPlayerCharacter::LookUp(float Value)
{    if (IsAlive())    {        AddControllerPitchInput(Value);    }}

void AProjectPlayerCharacter::LookUpRate(float Value)
{    if (IsAlive())    {        AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->DeltaTimeSeconds);    }}

void AProjectPlayerCharacter::Turn(float Value)
{    if (IsAlive())    {        AddControllerYawInput(Value);    }}

void AProjectPlayerCharacter::TurnRate(float Value)
{    if (IsAlive())    {        AddControllerYawInput(Value * BaseTurnRate * GetWorld()->DeltaTimeSeconds);    }}
*/
