// Test Game,  All Rights Reserved.

#include "AI/ProjectAICharacter.h"
#include "AI/PGAIController.h"
#include "ProjectUtils.h"
#include "Components/PGAIWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
// #include "UI/STUHealthBarWidget.h"
#include "Components/HelthComponent.h"
/*
AProjectAICharacter::AProjectAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = APGAIController::StaticClass();
}*/

AProjectAICharacter::AProjectAICharacter(const FObjectInitializer& ObjInit) 
 : Super(ObjInit.SetDefaultSubobjectClass<UPGAIWeaponComponent>("WeaponComponent"))
{
    //AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AutoPossessAI = EAutoPossessAI::Disabled;
    AIControllerClass = APGAIController::StaticClass();

    // AutoPossessAI = EAutoPossessAI::Disabled;

    bUseControllerRotationYaw = false;
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
    // HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    //  HealthWidgetComponent->SetupAttachment(GetRootComponent());
    // HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    //  HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void AProjectAICharacter::BeginPlay()

{
    // InventoryComponent->AddNewItem(Item, 1);
    Super::BeginPlay();
    // check(HealthWidgetComponent);
}

void AProjectAICharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // UpdateHealthWidgetVisibility();
}
/*
void AProjectAICharacter::UpdateHealthWidgetVisibility()
{
    if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;

    const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
    const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
    HealthWidgetComponent->SetVisibility(Distance < HealthVisibilityDistance, true);
}

void AProjectAICharacter::OnHealthChanged(float Health)
{
    Super::OnHealthChanged(Health);

    // GetUserWidgetObject() функция возвращает указатель на userwidget
    const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
    if (!HealthBarWidget) return;

    // HealthComponent береться из STUBaseCharacter
    HealthBarWidget->SetHealthPercent(HealthComponentQ->GetHealthPercent());
}*/

void AProjectAICharacter::OnDeath()
{
    Super::OnDeath();
    const auto PGController = Cast<AAIController>(Controller);
    if (PGController && PGController->BrainComponent)
    {
        PGController->BrainComponent->Cleanup();
    }
}