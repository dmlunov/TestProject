#pragma once
#include "Player/PGPlayerState.h"

#include "EditorLevelUtils.h"
#include "Editor/EditorEngine.h"
#include "Engine/LevelStreamingAlwaysLoaded.h"

//#include "ProjectUtils.generated.h"

class ProjectUtils
{

public:
    template <typename T>
    static T* GetProjectPlayerComponent(AActor* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }

       bool static AreEnemies(AController* Controller1, AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;
        const auto PlayerState1 = Cast<APGPlayerState>(Controller1->PlayerState);
        const auto PlayerState2 = Cast<APGPlayerState>(Controller2->PlayerState);

        return PlayerState1 && PlayerState2 && PlayerState1->GetTiamID() != PlayerState2->GetTiamID();
    }

    // функция преобразования
    static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }

    
   //// UFUNCTION()
   // void static AddSublevelToEditorWorld(FStrinf SublevelPath, bool& bOutSuccess, FString& OutInfoMessage)
   // {
   //     0;
   //   //  UWorld* World = GEditor->GetEditorWorldIdContext().World;

   // }
   //// UFUNCTION()
   // void static RemoveSublevelFromEditorWorld(FStrinf SublevelPath, bool& bOutSuccess, FString& OutInfoMessage)
   // {
   //     0;
   //     //
   // }
};

