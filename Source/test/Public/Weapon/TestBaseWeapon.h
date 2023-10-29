// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class TEST_API ATestBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATestBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
    FName SocketName = "MuzzleFlashSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float TraceMaxDistance = 3000.0f;


     

    virtual void MakeShot();
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;


    bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

    APlayerController* GetPlayerController() const;

    FVector GetMuzzleWorldLocation() const;

  

    void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);


private:

};
