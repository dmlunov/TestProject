// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

class AProjectBaseCharacter;
class UInventoryActionMessage;
class UInventoryPanel;

UCLASS()
class PROJECTGAME_API UMainMenu : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY()
    AProjectBaseCharacter* BaseCharacter;

   UFUNCTION()
    FORCEINLINE UInventoryActionMessage* GetInventoryActionMessageWidget() const { return InventoryActionMessage; };
   
    UPROPERTY(meta = (BindWidget))
    UInventoryActionMessage* InventoryActionMessage;

    UPROPERTY(meta = (BindWidget))
    UInventoryPanel* InventoryPanel;
protected:

     UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UInventoryActionMessage> InventoryActionMessageClass;

    virtual void NativeOnInitialized() override;
    virtual void NativeConstruct() override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
