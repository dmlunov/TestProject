// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TestBaseInterface.generated.h"

class ATestBaseCharacter;

UENUM()
enum class EInteractType : uint8
{
    Pickup UMETA(DisplayName = "Pickup"),
    NonPlayerCharacter UMETA(DisplayName = "NonPlayerCharacter"),
    Device UMETA(DisplayName = "Device"),
    Toggle UMETA(DisplayName = "Toggle"),
    Container UMETA(DisplayName = "Container")
};

USTRUCT()
struct FInteractableData
{
    GENERATED_USTRUCT_BODY()
    FInteractableData()
        : InteractType(EInteractType::Pickup),  //
          Name(FText::GetEmpty()),              //
          Action(FText::GetEmpty()),            //
          Quantity(0),                          //
          InteractionDuration(0.0f)
    {};

    UPROPERTY(EditInstanceOnly)
    EInteractType InteractType;

    UPROPERTY(EditInstanceOnly)
    FText Name;

    UPROPERTY(EditInstanceOnly)
    FText Action;

    UPROPERTY(EditInstanceOnly)
    int32 Quantity;

    UPROPERTY(EditInstanceOnly)
    float InteractionDuration;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTestBaseInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class TEST_API ITestBaseInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual void BeginFocus();
    virtual void EndFocus();
    virtual void BeginInteract();
    virtual void EndInteract();
    virtual void Interact(ATestBaseCharacter* BaseCharacter);

    FInteractableData InteractableData;
};
