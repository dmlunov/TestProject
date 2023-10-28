// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TestGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class TEST_API ATestGameHUD : public AHUD
{
	GENERATED_BODY()
public:

	virtual void DrawHUD() override;

private:
    void DrawCross();


};
