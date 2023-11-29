// Test Game,  All Rights Reserved.

#include "Player/ProjectBaseCharacter.h"
#include "Components/TestWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TestCharacterMovementComponent.h"
#include "Components/HelthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TestItemComponent.h"
#include "GameFramework/Controller.h"
#include "DrawDebugHelpers.h"
#include "Components/TestInventoryComponent.h"
#include "UI/TestGameHUD.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
AProjectBaseCharacter::AProjectBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UTestCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(-90.0f, -100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    WeaponComponent = CreateDefaultSubobject<UTestWeaponComponent>("WeaponComponent");

    HelthComponent = CreateDefaultSubobject<UHelthComponent>("HelthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    ItemComponent = CreateDefaultSubobject<UTestItemComponent>("ItemComponent");
    InventoryComponent = CreateDefaultSubobject<UTestInventoryComponent>("InventoryComponent");
    InventoryComponent->SetSlotsCapacity(20);
    InventoryComponent->SetWeightCapacity(50.0f);
}

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
}

// Called every frame
void AProjectBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
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
}

void AProjectBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
};

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

void AProjectBaseCharacter::OnDeath()
{

    // UE_LOG(BaseCharacterLog, Display, TEXT("Player is death, Name: %s"), *GetName());

    PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    WeaponComponent->StopFire();
}

void AProjectBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void AProjectBaseCharacter::ToggleMenu()
{
    TestGameHUD->ToggleMenu();
}