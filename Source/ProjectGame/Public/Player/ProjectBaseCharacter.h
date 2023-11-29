// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHelthComponent;
class UTextRenderComponent;
class UTestWeaponComponent;
class UTestItemComponent;
class UTestInventoryComponent;
class ATestGameHUD;

UCLASS()
class PROJECTGAME_API AProjectBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AProjectBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDerection() const;

    UPROPERTY(EditDefaultsOnly, Category = "Character | Animation_1")
    UAnimMontage* DeathAnimMontage;

    UFUNCTION()
    FORCEINLINE UTestInventoryComponent* GetInventoryComponent() const { return InventoryComponent; };
    UFUNCTION()
    FORCEINLINE UTestItemComponent* GetItemComponent() const { return ItemComponent; };
    UFUNCTION()
    FORCEINLINE UTestWeaponComponent* GetWeaponComponent() const { return WeaponComponent; };
    UPROPERTY()
    ATestGameHUD* TestGameHUD;
    

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

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UTestInventoryComponent* InventoryComponent;

 

private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void OnDeath();
    void OnHealthChanged(float Health);


    void ToggleMenu() ;
};
