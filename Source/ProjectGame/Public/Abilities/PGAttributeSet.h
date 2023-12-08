// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "PGAttributeSet.generated.h"

// Uses macros from Attributes.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName)                                                                                       \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)                                                                             \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)                                                                                           \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName);

UCLASS()
class PROJECTGAME_API UPGAttributeSet : public UAttributeSet
{
    GENERATED_BODY()

public:
    UPGAttributeSet();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    //virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    //virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    /** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
    UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
    FGameplayAttributeData Health;
    ATTRIBUTE_ACCESSORS(UPGAttributeSet, Health);


    UFUNCTION()
    virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

};
