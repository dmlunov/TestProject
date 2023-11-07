// Test Game,  All Rights Reserved.


#include "Components/TestItemComponent.h"
#include "Player/TestBaseCharacter.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UTestItemComponent::UTestItemComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	InteractionCheckFrequency = 0.1;
    InteractionCheckDistance = 225.0f;

}


// Called when the game starts
void UTestItemComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UTestItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	 if (GetWorld()->TimeSince(InteractionData.LasaerInteractionCheckTime) > InteractionCheckFrequency)
    {
        PerformInteractionCheck();
    }
}



void UTestItemComponent::PerformInteractionCheck()
{
    //
    ATestBaseCharacter* Character = Cast<ATestBaseCharacter>(GetOwner());

    FVector  PawnViewLocation = Character->GetPawnViewLocation();
    FRotator PawnViewRotation = Character->GetViewRotation();
    FVector PawnForwardVector = Character->GetActorForwardVector();

    InteractionData.LasaerInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector TraceStart{PawnViewLocation};
    FVector TraceEnd{TraceStart + (PawnViewRotation.Vector() * InteractionCheckDistance)};

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);
    FHitResult TraceHit;

    double LookDirection{FVector::DotProduct(PawnForwardVector, PawnViewRotation.Vector())};

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

void UTestItemComponent::FoundInteracteble(AActor* NewInteractable)
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
void UTestItemComponent::NoInteractableFound()
{
    UWorld* World = GetOwner()->GetWorld();

    if (IsInteracting())
    {
        World->GetTimerManager().ClearTimer(TimerHandle_Interaction);
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
void UTestItemComponent::BeginInteract()
{
    UWorld* World = GetOwner()->GetWorld();

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
                World->GetTimerManager().SetTimer(                             //
                    TimerHandle_Interaction,                                   //
                    this,                                                      //
                    &UTestItemComponent::Interact,                             //
                    TargetInteractable->InteractableData.InteractionDuration,  //
                    false);
            }
        }
    }
    //
}
void UTestItemComponent::EndInteract()
{
    UWorld* World = GetOwner()->GetWorld();

    World->GetTimerManager().ClearTimer(TimerHandle_Interaction);
    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->EndInteract();
    }
}
void UTestItemComponent::Interact()
{

    UWorld* World = GetOwner()->GetWorld();

    World->GetTimerManager().ClearTimer(TimerHandle_Interaction);
    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact();
    }
}