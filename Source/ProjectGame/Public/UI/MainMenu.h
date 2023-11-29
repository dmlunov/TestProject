// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class AProjectBaseCharacter;


UCLASS()
class PROJECTGAME_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY()
    AProjectBaseCharacter* BaseCharacter;


protected:
    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
