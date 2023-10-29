// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/TestBaseWeapon.h"
#include "TestLauncherWeapon.generated.h"


class ATestProjecttile;

UCLASS()
class TEST_API ATestLauncherWeapon : public ATestBaseWeapon
{
	GENERATED_BODY()
public:
    virtual void StartFire() override;

protected:

    virtual void MakeShot() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<ATestProjecttile> ProjecttileClass;
};
