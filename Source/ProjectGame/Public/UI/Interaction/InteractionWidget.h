// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class AProjectBaseCharacter;
struct FInteractableData;
class UTextBlock;
class UProgressBar;

UCLASS()
class PROJECTGAME_API UInteractionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "Interaction Widget | Player Reference")
    AProjectBaseCharacter* BaseCharacter;

    void UpdateWidget(const FInteractableData* InteractableData) const;

protected:
    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    UTextBlock* NameText;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    UTextBlock* ActionText;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    UTextBlock* QuantityText;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    UTextBlock* KeyPressText;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    UProgressBar* InteractionProgressBar;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Interaction Widget | Interactable Data")
    float CurrentInteractionDuration;

    UFUNCTION (Category = "Interaction")
    float UpdateInteractionProgress();

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;

};
