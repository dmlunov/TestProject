// Test Game,  All Rights Reserved.

#include "Weapon/TestProjecttile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/PGWeaponFXComponent.h"


DEFINE_LOG_CATEGORY_STATIC(ProjectTileLog, All, All);

ATestProjecttile::ATestProjecttile()
{

    PrimaryActorTick.bCanEverTick = false;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpherComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    CollisionComponent->bReturnMaterialOnMove = true;

    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;

    MovementComponent->ProjectileGravityScale = 0.5f;

     WeaponFXComponent = CreateDefaultSubobject<UPGWeaponFXComponent>("WeaponFXComponent");
}

void ATestProjecttile::BeginPlay()
{
    Super::BeginPlay();
    check(MovementComponent);
    check(CollisionComponent);
    check(WeaponFXComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATestProjecttile::OnProjecttileHit);

    SetLifeSpan(LifeProgectile);
}

void ATestProjecttile::OnProjecttileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld()) return;
    MovementComponent->StopMovementImmediately();

    FVector CenterHitLocation = GetActorLocation();

    TArray<FHitResult> HitResults;
    TArray<FVector> ImpulseDirections;
    TArray<AActor*> ActorsToIgnore{GetOwner()};

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, CenterHitLocation, DamageRadius, UDamageType::StaticClass(), {GetOwner()},this, GetController(), DoFullDamage);
    
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActors(ActorsToIgnore);

    // Найти все AActor, попавшие в сферу
    GetWorld()->SweepMultiByChannel(                //
        HitResults,                                 //
        CenterHitLocation,                          //
        CenterHitLocation,                          //
        FQuat::Identity,                            //
        ECollisionChannel::ECC_Pawn,                // 
        FCollisionShape::MakeSphere(DamageRadius),  //
        CollisionParams);
    FString ActorsHit;
 
    if (OtherActor && OtherComp == OtherActor->GetRootComponent())
    {
        FVector ImpulseDirection = ShotDirection * HitImpulse;
        OtherComp->AddImpulse(ImpulseDirection, NAME_None, true);
        
    }
    
    for (const FHitResult& hitactor : HitResults)
    {
        AActor* Actor = hitactor.GetActor();
        if (Actor)
        {
            //ActorsHit += " / " + Actor->GetName();
           
            UPrimitiveComponent* ActorComponent = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
            if (ActorComponent && ActorComponent->IsSimulatingPhysics())
            {
                FVector DirectionVector = ActorComponent->GetCenterOfMass() - CenterHitLocation;
                FVector HitVector = DirectionVector.GetSafeNormal() * (DamageRadius - DirectionVector.Length()) * HitImpulse/100.0f;
                ActorComponent->AddImpulse(HitVector, NAME_None, true);
                //UE_LOG(ProjectTileLog, Display, TEXT("Projectile Hit Name = %s"), *Actor->GetName());
            }
            
        }
    }

   // DrawDebugSphere(GetWorld(), CenterHitLocation, DamageRadius, 24, FColor::Red, false, 0.5f);
    WeaponFXComponent->PlayImpactFX(Hit);

    Destroy();
}

AController* ATestProjecttile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
};