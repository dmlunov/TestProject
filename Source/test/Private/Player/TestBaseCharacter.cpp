// Test Game,  All Rights Reserved.

#include "Player/TestBaseCharacter.h"
#include "Components/TestWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TestCharacterMovementComponent.h"
#include "Components/HelthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ATestBaseCharacter::ATestBaseCharacter(const FObjectInitializer& ObjInit)
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

    WeaponComponent = CreateDefaultSubobject <UTestWeaponComponent> ("WeaponComponent");

    HelthComponent = CreateDefaultSubobject<UHelthComponent>("HelthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);
    //
}

// Called when the game starts or when spawned
void ATestBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    check(HelthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HelthComponent->GetHealth());
    HelthComponent->OnDeath.AddUObject(this, &ATestBaseCharacter::OnDeath);
    HelthComponent->OnHealthChanged.AddUObject(this, &ATestBaseCharacter::OnHealthChanged);
}

// Called every frame
void ATestBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

// Called to bind functionality to input
void ATestBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ATestBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATestBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ATestBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ATestBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATestBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ATestBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ATestBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UTestWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UTestWeaponComponent::StopFire);
    PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UTestWeaponComponent::NextWeapon);

}

void ATestBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorForwardVector(), Amount);
};

void ATestBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
};

void ATestBaseCharacter::OnStartRunning()
{
    WantsToRun = true;
};
void ATestBaseCharacter::OnStopRunning()
{
    WantsToRun = false;
};

bool ATestBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
};

float ATestBaseCharacter::GetMovementDerection() const
{
    if (GetVelocity().IsZero()) return 0.0f;

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngelBetween);

    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
};

void ATestBaseCharacter::OnDeath()
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

void ATestBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
