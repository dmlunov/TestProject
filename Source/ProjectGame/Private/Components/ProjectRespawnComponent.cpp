// Test Game,  All Rights Reserved.


#include "Components/ProjectRespawnComponent.h"
#include "TestGameModeBase.h"



UProjectRespawnComponent::UProjectRespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UProjectRespawnComponent::Respawn(int32 RespawnTime)
{
    if (!GetWorld()) return;
    RespawnCountDown = RespawnTime;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &UProjectRespawnComponent::RespawnTimerUpdate, 1.0f, true);
}
void UProjectRespawnComponent::RespawnTimerUpdate()
{
    if (--RespawnCountDown == 0)
    {
        if (!GetWorld()) return;
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);

        const auto GameMode = Cast<ATestGameModeBase>(GetWorld()->GetAuthGameMode());
        if (!GameMode) return;

        GameMode->RespawnRequest(Cast<AController>(GetOwner()));
    }
}

bool UProjectRespawnComponent::IsRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

