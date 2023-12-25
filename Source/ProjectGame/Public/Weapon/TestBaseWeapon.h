// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectCoreTypes.h"
#include "TestBaseWeapon.generated.h"

class USkeletalMeshComponent;
class USoundCue;
class UNiagaraSystem;
class UNiagaraComponent;



UCLASS()
class PROJECTGAME_API ATestBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATestBaseWeapon();

    FOnClipEmptySignature OnClipEmpty;

    virtual void StartFire();
    virtual void StopFire();
    void ChangeClip();
    bool CanReload() const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool IsWeaponInInventory = false;

    FWeaponUIData GetUIData() const { return UIData; };
    FAmmoData GetAmmoData() const { return CurrentAmmo; };

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    FName SocketName = "MuzzleFlashSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 3000.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo {15, 10, false};


    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FWeaponUIData UIData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    UNiagaraSystem* MuzzleFX;

    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    APlayerController* GetPlayerController() const;

    FVector GetMuzzleWorldLocation() const;

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    void DecreaseAmmo();
    bool IsAmmoEmpty() const;
    bool IsClipEmpty() const;
    
    void LogAmmo();

    UNiagaraComponent* SpawnMuzzleFX();


private:
    FAmmoData CurrentAmmo;
};
