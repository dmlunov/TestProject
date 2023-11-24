// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TestPlayerHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API UTestPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

 UFUNCTION(BlueprintCallable, Category = "UI")
        float GetHealthPercent() const;

	
};
