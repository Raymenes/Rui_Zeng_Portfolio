// Fill out your copyright notice in the Description page of Project Settings.

#include "TetrisClimber.h"
#include "Projectile.h"
#include "Weapon.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;
}

AProjectile::AProjectile(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	InitialLifeSpan = 3.0f;
	// Use a sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	RootComponent = CollisionComp;
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &AProjectile::OnEndOverlap);

	//projectileMesh
	ProjectileMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("ProjectileMeshComp"));
	ProjectileMesh->AttachTo(RootComponent);


	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	//TheMaterial = ObjectInitializer.CreateDefaultSubobject<UMaterialInterface>(this, TEXT("MaterialComp"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AProjectile::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovement)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
	}
}


void AProjectile::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor && (OtherActor != this) && OtherComp != nullptr)
	{
		Destroy();
		ATCube* cube = dynamic_cast<ATCube*>(OtherActor);
		if (cube)
		{
			if (cube->mParent && canAct)
			{
				switch (mOwner->GetProjectileType())
				{
				case 0:
                        if (mActionType == Projectile_Action::T_Rotation) {
                            cube->mParent->Rotate(Tetris_Direction::T_Forward);
                        }
                        else if(mActionType == Projectile_Action::T_Movement){
                            cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Forward);
                        }
					break;
				case 1:
                        if (mActionType == Projectile_Action::T_Rotation) {
                            cube->mParent->Rotate(Tetris_Direction::T_Right);
                        }
                        else if(mActionType == Projectile_Action::T_Movement){
                            cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Right);
                        }
					break;
				case 2:
                        if (mActionType == Projectile_Action::T_Rotation) {
                            cube->mParent->Rotate(Tetris_Direction::T_Backward);
                        }
                        else if(mActionType == Projectile_Action::T_Movement){
                            cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Backward);
                        }
					break;
				default:
                        if (mActionType == Projectile_Action::T_Rotation) {
                            cube->mParent->Rotate(Tetris_Direction::T_Left);
                        }
                        else if(mActionType == Projectile_Action::T_Movement){
                            cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Left);
                        }
					break;
				}
                canAct = false;
			}
		}
		
	}
}
void AProjectile::OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && (OtherActor != this) && OtherComp != nullptr)
	{
		Destroy();
		
	}
}
/*
void AProjectile::ToggleUp()
{
	mProjType = UP;
}

void AProjectile::ToggleRight()
{
	mProjType = RIGHT;
	ProjectileMesh->SetMaterial(0, TheMaterial);
	//int i = 0;
	
	
}

void AProjectile::ToggleDown()
{
	mProjType = DOWN;
}

void AProjectile::ToggleLeft()
{
	mProjType = LEFT;
}
*/