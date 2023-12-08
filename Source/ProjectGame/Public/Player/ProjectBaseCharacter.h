// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCoreTypes.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GamePlayEffectTypes.h"
#include "ProjectBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
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

    /****************************
               GAS / AbilitySystem
    *****************************/
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
    virtual void AddStartupGameplayAbilities();


    virtual void PossessedBy(AController* NewControler) override;
    virtual void OnRep_PlayerState() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
    TObjectPtr<UPGAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GASGameplayAbility")
    TObjectPtr<UPGAttributeSet> Attributes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GASGameplayAbility")
    TArray<TSubclassOf<UPGGameplayAbility>> GameplayAbilities;

    // UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
    // TSubclassOf < UPGGameplayAbility > InitialAbilit;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
    TArray <TSubclassOf<UGameplayEffect>> PassiveGameplayEffects;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GASGameplayAbility")
    TSubclassOf<UGameplayEffect> DefaultAbilitEffect;

    //Благодаря использованию битового поля, достигается экономия памяти, так как булевский тип по умолчанию занимает 1 байт, но в данном случае используется всего 1 бит.
    UPROPERTY()
    uint8 bAbilitiesInitialized : 1;

 
    /**
     * Вызывается, когда персонаж получает урон, который мог его убить
     *
     * @param DamageAmount Количество нанесенного урона, не фиксируемое на основе текущего здоровья.
     * @param HitInfo Информация о попадании, вызвавшем этот урон.
     * @param DamageTags Теги игрового процесса события, причинившего ущерб.
     * @param InstigatorCharacter Персонаж, нанесший этот урон.
     * @param DamageCauser Фактическим действующим лицом, нанесшим урон, может быть оружие или снаряд.
     */
    UFUNCTION(BlueprintImplementableEvent)
    void OnDamagedGAS(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
        AProjectBaseCharacter* InstigatorCharacter, AActor* DamageCauser);

    /**
     * Вызывается при изменении здоровья в результате лечения или повреждения.
     * Для урона это вызывается в дополнение к OnDamaged/OnKilled.
     *
     * @param DeltaValue Изменение значения здоровья, положительное для лечения, отрицательное для стоимости. Если 0, дельта неизвестна.
     * @param EventTags Теги игрового процесса события, которое изменило ману.
     */
    UFUNCTION(BlueprintImplementableEvent)
    void OnHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);


    /** Вызываются из HopperAttributeSet, они вызывают события BP, указанные выше */
    virtual void HandleDamage(float DamageAmount, const FHitResult& HitInfo, const FGameplayTagContainer& DamageTags,
        AProjectBaseCharacter* InstigatorCharacter, AActor* DamageCauser);

    virtual void HandleHealthChanged(float DeltaValue, const FGameplayTagContainer& EventTags);

    friend UPGAttributeSet;


private:
    bool WantsToRun = false;
    bool IsMovingForward = false;

    void MoveForward(float Amount);
    void MoveRight(float Amount);
    void OnStartRunning();
    void OnStopRunning();
    void OnDeath();
    void OnHealthChanged(float Health);

    void ToggleMenu();
};
