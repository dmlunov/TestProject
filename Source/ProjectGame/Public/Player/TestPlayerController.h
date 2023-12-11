// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class ATestGameHUD;



UCLASS()
class PROJECTGAME_API ATestPlayerController : public APlayerController
{
    GENERATED_BODY()
protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

        UPROPERTY()
    ATestGameHUD* TestGameHUD;

    public:

        ATestGameHUD* GetHUD();
};
