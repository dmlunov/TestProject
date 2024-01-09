// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/TestBaseInterface.h"
#include "TestItemComponent.generated.h"

class ATestGameHUD;
class UItemBase;
class AProjectBaseCharacter;

USTRUCT()
struct FInteractionData
{
    GENERATED_USTRUCT_BODY()

    FInteractionData() : CurrentInteractable(nullptr), LasaerInteractionCheckTime(0.0f){};
    UPROPERTY()
    AActor* CurrentInteractable;

    UPROPERTY()
    float LasaerInteractionCheckTime;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTGAME_API UTestItemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTestItemComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    FORCEINLINE bool IsInteracting() const { return GetOwner()->GetWorld()->GetTimerManager().IsTimerActive(TimerHandle_Interaction); }


    void PerformInteractionCheck();
    void FoundInteracteble(AActor* NewInteractable);
    void NoInteractableFound();
    void BeginInteract();
    void EndInteract();
    void Interact();

    void UpdateInteractionWidget() const;

    void DropItem(UItemBase* ItemToDrop, const int32 QuantityToDrop); 

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    UPROPERTY()
    ATestGameHUD* TestGameHUD;


    UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
    TScriptInterface<ITestBaseInterface> TargetInteractable;

    

    float InteractionCheckFrequency;

    float InteractionCheckDistance;

    FTimerHandle TimerHandle_Interaction;

    FInteractionData InteractionData;

     AProjectBaseCharacter* Character;
};
