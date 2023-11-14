// Test Game,  All Rights Reserved.

#include "Components/TestItemComponent.h"
#include "Player/TestBaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "UI/TestGameHUD.h"
#include "Items/ItemBase.h"
#include "Components/TestInventoryComponent.h"
#include "Environment/TestPicupActor.h"
#include "Player/TestPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(TestItemComponentLog, All, All);

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

    TestGameHUD = Cast<ATestGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    Character = Cast<ATestBaseCharacter>(GetOwner());
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

    /*
        // 1 вариант выбора цели
        FVector PawnViewLocation = Character->GetPawnViewLocation();
        FRotator PawnViewRotation = Character->GetViewRotation();
        FVector PawnForwardVector = Character->GetActorForwardVector();
        */

    // 2 вариант выбора цели
    FVector PawnViewLocation;
    FRotator PawnViewRotation;
    const auto Pleyer = Cast<ACharacter>(GetOwner());
    if (!Pleyer) return;
    const auto Controller = Pleyer->GetController<ATestPlayerController>();
    if (!Controller) return;
    Controller->GetPlayerViewPoint(PawnViewLocation, PawnViewRotation);
    const FVector PawnForwardVector = PawnViewRotation.Vector();

     //PawnViewLocation = Character->GetPawnViewLocation();

    InteractionData.LasaerInteractionCheckTime = GetWorld()->GetTimeSeconds();

    FVector TraceStart{PawnViewLocation};
    FVector TraceEnd{TraceStart + (PawnViewRotation.Vector() * InteractionCheckDistance*2.5f)};

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);
    FHitResult TraceHit;

    double LookDirection{FVector::DotProduct(PawnForwardVector, PawnViewRotation.Vector())};

    if (LookDirection > 0)
    {

        //DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

        if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
        {
            if (!TraceHit.GetActor()) return;
            if (TraceHit.GetActor()->GetClass()->ImplementsInterface(UTestBaseInterface::StaticClass()))
            {
                // const float Distance = (TraceStart - TraceHit.ImpactPoint).Size();
                if (TraceHit.GetActor() != InteractionData.CurrentInteractable)  // && Distance <= InteractionCheckDistance)
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

    // update interaction widget
    TestGameHUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);

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

        TestGameHUD->HideInteractionWidget();

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
    // ATestBaseCharacter* BaseCharacter = Cast<ATestBaseCharacter>(GetOwner());

    World->GetTimerManager().ClearTimer(TimerHandle_Interaction);
    if (IsValid(TargetInteractable.GetObject()))
    {
        TargetInteractable->Interact(Character);
    }
}

void UTestItemComponent::UpdateInteractionWidget() const
{
    if (IsValid(TargetInteractable.GetObject()))
    {
        TestGameHUD->UpdateInteractionWidget(&TargetInteractable->InteractableData);
    }
}

void UTestItemComponent::DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop)
{
    UTestInventoryComponent* PlayerInventory = Character->GetInventory();

    if (PlayerInventory->FindMatchingItem(ItemToDrop))
    {

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = Character;
        SpawnParams.bNoFail = true;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

        // FItemData* ItemData = ItemDataTable->FindRow<FItemData>(DesiredItemID, DesiredItemID.ToString());

        const FRotator SpawnRotation{Character->GetActorRotation()};
        const FVector SpawnLocation{
            Character->GetActorLocation() + (Character->GetActorForwardVector() * 100.0f) + FVector(0.0f, 0.0f, 50.0f)};
        // const FTransform Transform {ItemToDrop->Transform};
        const FTransform ItemTransform{ItemToDrop->Transform};
        // UE_LOG(TestItemComponentLog, Display, TEXT("Transform Scale = %s"), *ItemTransform.GetScale3D().ToString());

        const FTransform SpawnTransform(SpawnRotation, SpawnLocation, ItemTransform.GetScale3D());

        ItemToDrop->AssetData.StaticMesh;
        const int32 RemovedQuantity = PlayerInventory->RemoveAmountOfItem(ItemToDrop, QuantityToDrop);

        ATestPicupActor* Pickup = GetWorld()->SpawnActor<ATestPicupActor>(ATestPicupActor::StaticClass(), SpawnTransform, SpawnParams);
        Pickup->InitializeDrop(ItemToDrop, RemovedQuantity);
        const float Massa{ItemToDrop->ItemPhysicalMass};
        Pickup->GetPickupMesh()->SetMassOverrideInKg(NAME_None, Massa);
    }
    else
    {
        UE_LOG(TestItemComponentLog, Display, TEXT("Item to drop was somehow null!"));
    }
}