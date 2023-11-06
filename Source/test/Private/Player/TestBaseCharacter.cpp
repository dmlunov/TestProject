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
#include "DrawDebugHelpers.h"

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

    WeaponComponent = CreateDefaultSubobject<UTestWeaponComponent>("WeaponComponent");

    HelthComponent = CreateDefaultSubobject<UHelthComponent>("HelthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    InteractionCheckFrequency = 0.1;
    InteractionCheckDistance = 225.0f;
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

    if (GetWorld()->TimeSince(InteractionData.LasaerInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
    // TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

void ATestBaseCharacter::PerformInteractionCheck()
{
    //
    InteractionData.LasaerInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector TraceStart{GetPawnViewLocation()};
    FVector TraceEnd{TraceStart + (GetViewRotation().Vector() * InteractionCheckDistance)};

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);
    FHitResult TraceHit;

    double LookDirection{FVector::DotProduct(GetActorForwardVector(), GetViewRotation().Vector())};

    if (LookDirection > 0)
    {

        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

        if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            if (!TraceHit.GetActor()) return;
            if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UTestBaseInterface::StaticClass()))
            {
                const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
                if (TraceHit.GetActor() != InteractionData.CurrentInteractable && Distance <= InteractionCheckDistance)
                {
                    FoundInteracteble(TraceHit.GetActor());
                    return;
                }
                if (TraceHit.GetActor() == InteractionData.CurrentInteractable) return;
            }
        }
    }
    NoInteractableFound();
}

void ATestBaseCharacter::FoundInteracteble(AActor* NewInteractable)
{
    if (IsInteracting())
    {
        EndInteract();
    }
    if (InteractionData.CurrentInteractable)
    {
        TargetInteractable = InteractionData.CurrentInteractable;
        TargetInteractable->EndFocus();
    }
    InteractionData.CurrentInteractable = NewInteractable;
    TargetInteractable = NewInteractable;

    TargetInteractable->BeginFocus();
}
void ATestBaseCharacter::NoInteractableFound()
{
    if (IsInteracting())
    {
        GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    }
    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->EndFocus();
        }
        //

        InteractionData.CurrentInteractable = nullptr;
        TargetInteractable = nullptr;
    }
}
void ATestBaseCharacter::BeginInteract()
{
    PerformInteractionCheck();
    if (InteractionData.CurrentInteractable)
    {
        if (IsValid(TargetInteractable.GetObject()))
        {
            TargetInteractable->BeginInteract();
            if (FMath::IsNearlyZero(TargetInteractable->InteractableData.InteractionDuration, 0.1f))
            {
                Interact();
            }
            else
            {
                GetWorldTimerManager().SetTimer(                               //
                    TimerHandle_Interaction,                                   //
                    this,                                                      //
                    &ATestBaseCharacter::Interact,                             //
                    TargetInteractable->InteractableData.InteractionDuration,  //
                    false);
            }
        }
    }
    //
}
void ATestBaseCharacter::EndInteract()
{

    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->EndInteract();
    }
}
void ATestBaseCharacter::Interact()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_Interaction);
    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact();
    }
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
    PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UTestWeaponComponent::Reload);

     PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ATestBaseCharacter::BeginInteract);
    PlayerInputComponent->BindAction("Interact", IE_Released, this, &ATestBaseCharacter::EndInteract);

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
