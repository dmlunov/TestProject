// Test Game,  All Rights Reserved.


#include "Player/TestPlayerController.h"
#include "UI/TestPlayerHUDWidget.h"
#include "UI/TestGameHUD.h"
#include "Player/PGPlayerState.h"
#include "Components/PGAbilitySystemComponent.h"
#include "Player/ProjectBaseCharacter.h"


DEFINE_LOG_CATEGORY_STATIC(PlayerControllerLog, All, All);

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();
   // TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
   // PlayerHUDWidget = TestGameHUD->GetPlayerHUDWidget();
}


ATestGameHUD* ATestPlayerController::GetHUD() const
{
    ATestGameHUD* TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    return TestGameHUD;
 }

UTestPlayerHUDWidget* ATestPlayerController::GetPlayerHUDWidget() const
 {
    return GetHUD()->GetPlayerHUDWidget();
}

// Server only
void ATestPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    APGPlayerState* PS = GetPlayerState<APGPlayerState>();
    if (PS)
    {
        // Init ASC with PS (Owner) and our new Pawn (AvatarActor)
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
    }
}

void ATestPlayerController::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // For edge cases where the PlayerState is repped before the Hero is possessed.
    CreateHUD();
}

 void ATestPlayerController::CreateHUD()
{
    if (!IsLocalPlayerController())
    {
        return;
    }

    // Need a valid PlayerState to get attributes from
    APGPlayerState* PS = GetPlayerState<APGPlayerState>();
    if (!PS)
    {
        return;
    }

   ATestGameHUD* TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    UTestPlayerHUDWidget* PlayerHUDWidget = TestGameHUD->GetPlayerHUDWidget();

    if (!PlayerHUDWidget) return;
	// Set attributes
    PlayerHUDWidget->SetCurrentHealth(PS->GetHealth());
    PlayerHUDWidget->SetMaxHealth(PS->GetMaxHealth());
    PlayerHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
    PlayerHUDWidget->SetCurrentMana(PS->GetMana());
    PlayerHUDWidget->SetMaxMana(PS->GetMaxMana());
    PlayerHUDWidget->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
    PlayerHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
    PlayerHUDWidget->SetManaRegenRate(PS->GetManaRegenRate());
    PlayerHUDWidget->SetCurrentStamina(PS->GetStamina());
    PlayerHUDWidget->SetMaxStamina(PS->GetMaxStamina());
    PlayerHUDWidget->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
    PlayerHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
    PlayerHUDWidget->SetExperience(PS->GetXP());
    PlayerHUDWidget->SetGold(PS->GetGold());
    PlayerHUDWidget->SetHeroLevel(PS->GetCharacterLevel());
    UE_LOG(PlayerControllerLog, Display, TEXT("Create HUD"));
}

 void ATestPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AProjectBaseCharacter* TargetCharacter)
{
    /*if (TargetCharacter && DamageNumberClass)
    {
         UGDDamageTextWidgetComponent* DamageText = NewObject<UGDDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
        DamageText->RegisterComponent();
        DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        DamageText->SetDamageText(DamageAmount);
    }*/
}

bool ATestPlayerController::ShowDamageNumber_Validate(float DamageAmount, AProjectBaseCharacter* TargetCharacter)
{
    return true;
}

void ATestPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
    ATestGameHUD* TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    UTestPlayerHUDWidget* PlayerHUDWidget = TestGameHUD->GetPlayerHUDWidget();

    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
    }
}

bool ATestPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
    return true;
}