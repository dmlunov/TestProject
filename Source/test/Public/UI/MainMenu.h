// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class ATestBaseCharacter;


UCLASS()
class TEST_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY()
    ATestBaseCharacter* BaseCharacter;


protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
