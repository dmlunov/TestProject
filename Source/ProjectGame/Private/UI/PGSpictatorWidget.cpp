// Test Game,  All Rights Reserved.


#include "UI/PGSpictatorWidget.h"
#include "Components/ProjectRespawnComponent.h"


bool UPGSpictatorWidget::GetRespawnTime(int32& CountDownTime) const
{
    const auto RespawnComponent = GetOwningPlayer()->GetComponentByClass<UProjectRespawnComponent>();
    if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;
    CountDownTime = RespawnComponent->GetRespawnCountDown();
    return true;
}
