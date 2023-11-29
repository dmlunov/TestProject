// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UBorder;
class UImage;
class UTextBlock;

UCLASS()
class PROJECTGAME_API UDragItemVisual : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
    UBorder* ItemBorder;

    UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
    UImage* ItemIcon;

    UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
    UTextBlock* ItemQuantity;

protected:
};
