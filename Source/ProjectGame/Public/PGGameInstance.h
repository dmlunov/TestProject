// Test Game,  All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectCoreTypes.h"
#include "PGGameInstance.generated.h"

class USoundClass;

UCLASS()
class PROJECTGAME_API UPGGameInstance : public UGameInstance
{
	
    GENERATED_BODY()

public:
    // FString TestString = "Hello World";
    FLevelData GetStartupLevel() const { return StartupLevel; }

    void SetStartupLevel(const FLevelData& Data) { StartupLevel = Data; }

    TArray<FLevelData> GetLevelsData() const { return LevelsData; }

    FName GetMenyLevelName() const { return MenyLevelName; }

    //void ToggleVolume();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game", meta = (ToolTip = "Level names must be unique!"))
    TArray<FLevelData> LevelsData;

    // UPROPERTY(EditDefaultsOnly, Category = "Game")//убрали в private:
    // FName StartupLevelName = NAME_None;// специальная переменная для инициальзации FName

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenyLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundClass* MasterSoundClass;

private:
    FLevelData StartupLevel;
};
