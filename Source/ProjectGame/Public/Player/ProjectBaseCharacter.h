// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCoreTypes.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GamePlayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "ProjectBaseCharacter.generated.h"

// class USpringArmComponent;
// class UCameraComponent;
class UHelthComponent;
class UTextRenderComponent;
class UTestWeaponComponent;
class UTestItemComponent;
class UTestInventoryComponent;
class ATestGameHUD;

class UPGAbilitySystemComponent;
class UPGAttributeSet;
class UPGGameplayAbility;
class UGameplayEffect;
struct FGameplayTagContainer;

UCLASS()
class PROJECTGAME_API AProjectBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    AProjectBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsTakeWeapon() const ;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDerection() const;

    // Only called on the Server. Calls before Server's AcknowledgePossession.
    virtual void PossessedBy(AController* NewController) override;

    UFUNCTION()
    FORCEINLINE UPGAttributeSet* GetAttributes() const { return Attributes.Get(); };

    UPROPERTY(EditDefaultsOnly, Category = "Character | Animation_1")
    UAnimMontage* DeathAnimMontage;

        UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    ATestGameHUD* TestGameHUD;

    virtual void OnDeath();

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character")
    virtual void FinishDying();

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character")
    virtual bool IsAlive() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (ClampMin = "0", ClampMax = "10"))
    int32 TeamID{0};

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

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

    /****************************
               GAS / AbilitySystem
    *****************************/

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    TObjectPtr<UPGAbilitySystemComponent> AbilitySystemComponent;
    TObjectPtr<UPGAttributeSet> Attributes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility|Abilities")
    TArray<TSubclassOf<UPGGameplayAbility>> CharacterAbilities;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility|Abilities")
    TSubclassOf<UGameplayEffect> DefaultAttributes;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility|Abilities")
    TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility|Character")
    FText CharacterName;

    UPROPERTY(BlueprintAssignable, Category = "GASGameplayAbility|Character")
    FCharacterDiedDelegate OnCharacterDied;

    FGameplayTag DeadTag;
    FGameplayTag EffectRemoveOnDeathTag;

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
    // TSubclassOf < UPGGameplayAbility > InitialAbilit;

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility|Abilities")
    // TArray<TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

    // Включите «AbilityID», чтобы вернуть индивидуальные уровни способностей.
    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character")
    virtual int32 GetAbilityLevel(EPGAbilityInputID AbilityID) const;

    // Удаляет все способности персонажа. Может быть вызван только Сервером. Удаление на сервере приведет и к удалению с клиента.
    virtual void RemoveCharacterAbilities();

    virtual void AddCharacterAbilities();

    virtual void InitializeAttributes();

    virtual void AddStartupEffects();

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    int32 GetCharacterLevel() const;

public:
    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetMana() const;

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetMaxMana() const;

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetStamina() const;

    UFUNCTION(BlueprintCallable, Category = "GASGameplayAbility|Character|Attributes")
    float GetMaxStamina() const;

    virtual void OnHealthChanged(float Health);
    virtual void SetHealth(float Health);
    virtual void SetMana(float Mana);
    virtual void SetStamina(float Stamina);



    void SetPlayerColor(const FLinearColor& Color);

    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);  // FHitResult структура в которой содержиться инфа об контакте двух обектов



 protected:
    bool WantsToRun = false;
    bool IsMovingForward = false;
};
