// Test Game,  All Rights Reserved.

#include "Weapon/PGPunchAttack.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Components/HelthComponent.h"
#include "Animation/PGPunchFinishAnimNotify.h"
#include "Animation/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(PGPunchAttackLog, All, All);

APGPunchAttack::APGPunchAttack()
{
    HitCollision = CreateDefaultSubobject<USphereComponent>("HitCollision");
    // SetRootComponent(HitCollision);
    HitCollision->SetupAttachment(GetRootComponent());
    // WeaponMesh->SetupAttachment(GetRootComponent());
    HitCollision->InitSphereRadius(30.0f);
    HitCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    // HitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    HitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // HitCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    HitCollision->IgnoreActorWhenMoving(GetOwner(), true);
    HitCollision->IgnoreActorWhenMoving(this, true);

    IsWeaponInInventory = true;

    HitActor = nullptr;
}

void APGPunchAttack::BeginPlay()
{
    Super::BeginPlay();

    //  HitCollision->OnComponentHit.AddDynamic(this, &APGPunchAttack::MakePunch);

    HitCollision->OnComponentBeginOverlap.AddDynamic(this, &APGPunchAttack::MakePunch);
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    InitAnimation();
}

void APGPunchAttack::StartFire()
{

    if (!CanPunch()) return;

    HitCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    if (!GetWorld()) return;

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    int32 RandomIndex = FMath::RandRange(0, PunchAnimMontages.Num() - 1);

    PunchAnimMontage = PunchAnimMontages[RandomIndex];

    if (!Character && !PunchAnimMontage) return;
    PunchAnimInProgress = true;

    Character->PlayAnimMontage(PunchAnimMontage);

    FVector PawnViewLocation = Character->GetPawnViewLocation();
    FRotator PawnViewRotation = Character->GetViewRotation() + FRotator{0, 30, 0};

    // FVector PawnForwardVector = Character->GetActorForwardVector();
    float DistanceHit = 200.0f;
    FVector TraceStart{PawnViewLocation};
    FVector TraceEnd{TraceStart + (PawnViewRotation.Vector() * DistanceHit)};

    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Character);
    FHitResult TraceHit;

    if (GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
    {
       // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);
        HitActor = TraceHit.GetActor();
    }

    //  UE_LOG(PGPunchAttackLog, Display, TEXT("Start Attack name Owner = %s"), *GetOwner()->GetName());
}

void APGPunchAttack::MakePunch  //
    (UPrimitiveComponent* OverlappComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
        const FHitResult& HitResult)
//(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& HitResult)
{

    if (!GetWorld()) return;

    // if (HitResult.bBlockingHit)
    // {

    // TraceFXEnd = HitResult.ImpactPoint;
    AActor* HitActorOverLap = HitResult.GetActor();
   // UE_LOG(PGPunchAttackLog, Display, TEXT(" OverLapActor: %s"), *HitActorOverLap->GetName());
    if (HitActor && HitActor->FindComponentByClass<UHelthComponent>() && HitActor != GetOwner())
    {
        if (!HitActors.Contains(HitActor))
        {

          //  UE_LOG(PGPunchAttackLog, Display, TEXT("Damage HitActor: %s"), *HitActor->GetName());
            HitActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
            HitActors.Add(HitActor);
        }
    }
    else if (HitActorOverLap && HitActorOverLap->FindComponentByClass<UHelthComponent>() && HitActorOverLap != GetOwner())
    {
        if (!HitActors.Contains(HitActorOverLap))
        {
            HitActorOverLap->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
            HitActors.Add(HitActorOverLap);
        }
    }
    if (HitActorOverLap != GetOwner())
    {
        HitActor = nullptr;
    }

    // SpawnMuzzleFX();
}

void APGPunchAttack::MakeDamage(const FHitResult& HitResult)
{
    const auto DamageActor = HitResult.GetActor();
    if (!DamageActor) return;
    DamageActor->TakeDamage(DamageAmount, FDamageEvent{}, GetPlayerController(), this);
}

void APGPunchAttack::InitAnimation()
{
    for (auto OnePunchAnimMontage : PunchAnimMontages)
    {
        auto PunchFinishedNotify = AnimUtils::FindNotifyByClass<UPGPunchFinishAnimNotify>(OnePunchAnimMontage);
        if (PunchFinishedNotify)
        {
            PunchFinishedNotify->OnNotified.AddUObject(this, &APGPunchAttack::OnPunchFinished);
        }
        else
        {
            UE_LOG(PGPunchAttackLog, Error, TEXT("Equip Animation notify is forgotten to set"));
            checkNoEntry();
        }
    }
}

void APGPunchAttack::OnPunchFinished(USkeletalMeshComponent* MeshComp)
{
    HitCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HitActors.Empty();
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || Character->GetMesh() != MeshComp) return;
    PunchAnimInProgress = false;
}

bool APGPunchAttack::CanPunch() const
{
    return !PunchAnimInProgress;
}