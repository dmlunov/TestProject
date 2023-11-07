// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
//#include "Interface/TestBaseInterface.h"
#include "TestBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHelthComponent;
class UTextRenderComponent;
class UTestWeaponComponent;
class UTestItemComponent;


/* USTRUCT()
struct FInteractionData
{
    GENERATED_USTRUCT_BODY()

    FInteractionData() : CurrentInteractable(nullptr), LasaerInteractionCheckTime(0.0f){};
    UPROPERTY()
    AActor* CurrentInteractable;

    UPROPERTY()
    float LasaerInteractionCheckTime;

};*/

UCLASS()
class TEST_API ATestBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:

    ATestBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDerection() const;

    UPROPERTY(EditDefaultsOnly, Category = "Character | Animation_1")
    UAnimMontage* DeathAnimMontage;

   // FORCEINLINE bool IsInteracting() const { return GetWorldTimerManager().IsTimerActive(TimerHandle_Interaction); }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UHelthComponent* HelthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTextRenderComponent* HealthTextComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTestWeaponComponent* WeaponComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTestItemComponent* ItemComponent;

/*
    // interact
    UPROPERTY(VisibleAnywhere, Category = "Character | Interaction")
    TScriptInterface<ITestBaseInterface> TargetInteractable;


    float InteractionCheckFrequency;

    float InteractionCheckDistance;

    FTimerHandle TimerHandle_Interaction;

    FInteractionData InteractionData;

    void PerformInteractionCheck();
    void FoundInteracteble(AActor* NewInteractable);
    void NoInteractableFound();
    void BeginInteract();
    void EndInteract();
    void Interact();

    //
*/
private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void OnDeath();
    void OnHealthChanged(float Health);
};
