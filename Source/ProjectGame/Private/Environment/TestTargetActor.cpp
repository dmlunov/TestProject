// Test Game,  All Rights Reserved.


#include "Environment/TestTargetActor.h"
#include "Components/HelthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

DEFINE_LOG_CATEGORY_STATIC(TestTagetActorLog, All, All);

ATestTargetActor::ATestTargetActor()
{
 
	PrimaryActorTick.bCanEverTick = false;

    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    StaticMeshComponent->SetupAttachment(GetRootComponent());
    HelthComponent = CreateDefaultSubobject<UHelthComponent>("HelthComponent");
    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(StaticMeshComponent);
    HelthComponent->SetAutoHeal(false);

}


void ATestTargetActor::BeginPlay()
{
	Super::BeginPlay();
    check(HelthComponent);
    check(HealthTextComponent);
    check(StaticMeshComponent);

    OnHealthChanged(HelthComponent->GetHealth(), 0.0f);

    HelthComponent->OnDeath.AddUObject(this, &ATestTargetActor::OnDeath);
    HelthComponent->OnHealthChanged.AddUObject(this, &ATestTargetActor::OnHealthChanged);

	MaxHealth = HelthComponent->GetMaxHealth();
}

void ATestTargetActor::OnDeath()
{
   // PlayAnimMontage(DeathAnimMontage);
   // GetCharacterMovement()->DisableMovement();
    SetLifeSpan(1.0f);
}

void ATestTargetActor::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));

    
    UMaterialInstanceDynamic* DynMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
    if (DynMaterial)
    {
        //DynMaterial->SetVectorParameterValue("HealthColor", Color);  // FLinearColor::Black3
        DynMaterial->SetScalarParameterValue(FName("HealthColor"), (MaxHealth-Health) / MaxHealth);
       // UE_LOG(TestTagetActorLog, Display, TEXT("Helth Color=%f"), (MaxHealth - Health) / MaxHealth);
    }

}




