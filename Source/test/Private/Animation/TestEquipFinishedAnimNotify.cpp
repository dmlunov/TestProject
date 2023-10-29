// Test Game,  All Rights Reserved.


#include "Animation/TestEquipFinishedAnimNotify.h"




void UTestEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}