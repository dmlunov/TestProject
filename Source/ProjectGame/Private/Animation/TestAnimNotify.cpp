// Test Game,  All Rights Reserved.


#include "Animation/TestAnimNotify.h"

void UTestAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}