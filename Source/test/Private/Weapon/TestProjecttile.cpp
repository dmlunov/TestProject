// Test Game,  All Rights Reserved.

#include "Weapon/TestProjecttile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ATestProjecttile::ATestProjecttile()
{

    PrimaryActorTick.bCanEverTick = false;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SpherComponent");
    CollisionComponent->InitSphereRadius(5.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
    MovementComponent->InitialSpeed = 2000.0f;

    MovementComponent->ProjectileGravityScale = 0.5f;
}

void ATestProjecttile::BeginPlay()
{
    Super::BeginPlay();
    check(MovementComponent);
    check(CollisionComponent);

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATestProjecttile::OnProjecttileHit);

    SetLifeSpan(LifeProgectile);
}

void ATestProjecttile::OnProjecttileHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!GetWorld()) return;
    MovementComponent->StopMovementImmediately();

    UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation(), DamageRadius, UDamageType::StaticClass(),
        {GetOwner()},this , GetController(), DoFullDamage);
    DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 0.5f);

    Destroy();
}

AController* ATestProjecttile::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
};