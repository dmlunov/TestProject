// Test Game,  All Rights Reserved.

#include "UI/Interaction/InteractionWidget.h"
#include "Interface/TestBaseInterface.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UInteractionWidget::NativeOnInitialized()
{

    Super::NativeOnInitialized();

    InteractionProgressBar->PercentDelegate.BindUFunction(this, "UpdateInteractionProgress");
}

void UInteractionWidget::NativeConstruct()
{

    Super::NativeConstruct();

    KeyPressText->SetText(FText::FromString("Press"));
    CurrentInteractionDuration = 0.0f;
}

void UInteractionWidget::UpdateWidget(const FInteractableData* InteractableData) const 
{
    switch (InteractableData->InteractType)
    {
        case EInteractType::Pickup:
            KeyPressText->SetText(FText::FromString("Press"));
            InteractionProgressBar->SetVisibility(ESlateVisibility::Collapsed);
            if (InteractableData->Quantity < 2)
            {
                QuantityText->SetVisibility(ESlateVisibility::Collapsed);
            }
            else
            {
                QuantityText->SetText(FText::Format(NSLOCTEXT("InteractionWidget", "QuantityText", "x{0}"),  //
                    InteractableData->Quantity));
                QuantityText->SetVisibility(ESlateVisibility::Visible);
            }

            break;

        case EInteractType::NonPlayerCharacter: break;

        case EInteractType::Device: break;

        case EInteractType::Toggle: break;

        case EInteractType::Container: break;

        default:;
    }
    ActionText->SetText(InteractableData->Action);
    NameText->SetText(InteractableData->Name);

}

float UInteractionWidget::UpdateInteractionProgress()
{
    return 0.0;
}
