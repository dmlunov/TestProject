// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

class UItemBase;
class UTestInventoryComponent;




UCLASS()
class PROJECTGAME_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:

	UPROPERTY()
    UItemBase* SourceItem;

	UPROPERTY()
    UTestInventoryComponent* SourceInventory;
};
