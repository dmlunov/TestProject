// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player/ProjectBaseCharacter.h"
#include "ProjectPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 *
 */
UCLASS()
class PROJECTGAME_API AProjectPlayerCharacter : public AProjectBaseCharacter
{
    GENERATED_BODY()
public:
    AProjectPlayerCharacter(const class FObjectInitializer& ObjectInitializer);

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Only called on the Server. Calls before Server's AcknowledgePossession.
    // virtual void PossessedBy(AController* NewController) override;

    USpringArmComponent* GetSpringArmComponent() { return SpringArmComponent; };  // GetCameraBoom

    UCameraComponent* GetCameraComponent() const { return CameraComponent; };  // GetFollowCamera

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|Camera")
    float GetCameraSpringArmLength() const { return CameraSpringArmLength; };

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|Camera")
    FVector GetCameraSpringArmLocation() const { return CameraSpringArmLocation; };

    //  class UGDFloatingStatusBarWidget* GetFloatingStatusBar();

    //   USkeletalMeshComponent* GetGunComponent() const;

    //    virtual void FinishDying() override;

protected:
    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASDocumentation|Camera")
    float BaseTurnRate = 45.0f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASDocumentation|Camera")
    float BaseLookUpRate = 45.0f;

    UPROPERTY(BlueprintReadOnly, Category = "GASDocumentation|Camera")
    float CameraSpringArmLength;

    UPROPERTY(BlueprintReadOnly, Category = "GASDocumentation|Camera")
    FVector CameraSpringArmLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* CameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArmComponent;

    UPROPERTY()
    bool ASCInputBound{false};

    FGameplayTag DeadTag;

    //   UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
    //  USkeletalMeshComponent* GunComponent;

    //  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GASDocumentation|UI")
    //  TSubclassOf<class UGDFloatingStatusBarWidget> UIFloatingStatusBarClass;

    // UPROPERTY()
    //  class UGDFloatingStatusBarWidget* UIFloatingStatusBar;

    // UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GASDocumentation|UI")
    //  class UWidgetComponent* UIFloatingStatusBarComponent;

    // Called when the game starts or when spawned

    /*
    // Mouse
    void LookUp(float Value);

    // Gamepad
    void LookUpRate(float Value);

    // Mouse
    void Turn(float Value);

    // Gamepad
    void TurnRate(float Value);

    */

    // Creates and initializes the floating status bar for heroes.
    // Safe to call many times because it checks to make sure it only executes once.
    // UFUNCTION()
    // void InitializeFloatingStatusBar();

    // Client only
    virtual void OnRep_PlayerState() override;

    // Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController
    // might call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState
    // would be null in SetupPlayerInputComponent. Conversely, the PlayerState might be repped before the PlayerController calls
    // ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
    void BindASCInput();

private:
    FTimerHandle StaminaTimerHandle;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void ToggleMenu();

    void StaminaUpdate();
};
