// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class ATestGameHUD;
class UTestPlayerHUDWidget;

UCLASS()
class PROJECTGAME_API ATestPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ATestGameHUD* GetHUD() const ;
    UTestPlayerHUDWidget* GetPlayerHUDWidget() const ;

    void CreateHUD();

   // UPROPERTY(EditAnywhere, Category = "GASDocumentation|UI")
   // TSubclassOf<class UGDDamageTextWidgetComponent> DamageNumberClass;

    class UGDHUDWidget* GetHUD();

    UFUNCTION(Client, Reliable, WithValidation)
    void ShowDamageNumber(float DamageAmount, AProjectBaseCharacter* TargetCharacter);
    void ShowDamageNumber_Implementation(float DamageAmount, AProjectBaseCharacter* TargetCharacter);
    bool ShowDamageNumber_Validate(float DamageAmount, AProjectBaseCharacter* TargetCharacter);

    // Simple way to RPC to the client the countdown until they respawn from the GameMode. Will be latency amount of out sync with the
    // Server.
    UFUNCTION(Client, Reliable, WithValidation)
    void SetRespawnCountdown(float RespawnTimeRemaining);
    void SetRespawnCountdown_Implementation(float RespawnTimeRemaining);
    bool SetRespawnCountdown_Validate(float RespawnTimeRemaining);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

   // UPROPERTY()
   // ATestGameHUD* TestGameHUD;
   // UTestPlayerHUDWidget* PlayerHUDWidget;

    // Server only
    virtual void OnPossess(APawn* InPawn) override;

    virtual void OnRep_PlayerState() override;

};
