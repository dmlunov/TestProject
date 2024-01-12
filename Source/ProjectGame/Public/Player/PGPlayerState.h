// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameplayEffectTypes.h"
#include "PGPlayerState.generated.h"

class UPGAttributeSet;
class UPGAbilitySystemComponent;
//class ATestPlayerController;
//class AProjectBaseCharacter;
//class UTestPlayerHUDWidget;
/**
 *
 */
UCLASS()
class PROJECTGAME_API APGPlayerState : public APlayerState, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    APGPlayerState();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UPGAttributeSet* GetAttributeSetBase() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState")
    bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumenation|GDPlayerState|UI")
    void ShowAbilityConfirmCancelText(bool ShowText);

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetHealth() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetMaxHealth() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetHealthRegenRate() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetMana() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetMaxMana() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetManaRegenRate() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetStamina() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetMaxStamina() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetStaminaRegenRate() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetArmor() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    float GetMoveSpeed() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    int32 GetCharacterLevel() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    int32 GetXP() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    int32 GetXPBounty() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    int32 GetGold() const;

    UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState|Attributes")
    int32 GetGoldBounty() const;

protected:
    UPROPERTY()
    UPGAbilitySystemComponent* AbilitySystemComponent;

    UPROPERTY()
    UPGAttributeSet* AttributeSetBase;

    FGameplayTag DeadTag;

   // AProjectBaseCharacter* CurentCharacter;
   // ATestPlayerController* PlayerController;
   // UTestPlayerHUDWidget* PlayerHUDWidget;

    FDelegateHandle HealthChangedDelegateHandle;
    FDelegateHandle MaxHealthChangedDelegateHandle;
    FDelegateHandle HealthRegenRateChangedDelegateHandle;
    FDelegateHandle ManaChangedDelegateHandle;
    FDelegateHandle MaxManaChangedDelegateHandle;
    FDelegateHandle ManaRegenRateChangedDelegateHandle;
    FDelegateHandle StaminaChangedDelegateHandle;
    FDelegateHandle MaxStaminaChangedDelegateHandle;
    FDelegateHandle StaminaRegenRateChangedDelegateHandle;
    FDelegateHandle XPChangedDelegateHandle;
    FDelegateHandle GoldChangedDelegateHandle;
    FDelegateHandle CharacterLevelChangedDelegateHandle;
  
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Attribute changed callbacks
     virtual void HealthChanged(const FOnAttributeChangeData& Data);
    virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
    virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
    virtual void ManaChanged(const FOnAttributeChangeData& Data);
    virtual void MaxManaChanged(const FOnAttributeChangeData& Data);
    virtual void ManaRegenRateChanged(const FOnAttributeChangeData& Data);
    virtual void StaminaChanged(const FOnAttributeChangeData& Data);
    virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
    virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
    virtual void XPChanged(const FOnAttributeChangeData& Data);
    virtual void GoldChanged(const FOnAttributeChangeData& Data);
    virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);

    // Tag change callbacks
    virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);


public:
    void SetTiamID(int32 ID) { TeamID = ID; };
    int32 GetTiamID() const { return TeamID; };
    void SetTeamColor(const FLinearColor& Color) { TeamColor = Color; };
    FLinearColor GetTeamColor() const { return TeamColor; };

    void AddKill() { ++KillsNum; };
    int32 GetKillsNum() const { return KillsNum; };

    void AddDeath() { ++DeathsNum; };
    int32 GetDeath() const { return DeathsNum; };

    void LogInfo();

private:
    int32 TeamID;
    FLinearColor TeamColor;

    int32 KillsNum = 0;
    int32 DeathsNum = 0;

};
