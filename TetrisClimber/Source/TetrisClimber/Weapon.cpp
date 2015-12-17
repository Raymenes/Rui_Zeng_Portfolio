// Fill out your copyright notice in the Description page of Project Settings.
#include "TetrisClimber.h"
#include "Sound/SoundCue.h"
#include "Weapon.h"
#include "TetrisClimberHUD.h"
#include "DrawDebugHelpers.h"
#include "TetrisManager.h"


// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AWeapon::OnStartFire()
{
	FireAC = PlayWeaponSound(FireLoopSound);
	// try and fire a projectile

	
	if (ProjectileClass != NULL)
	{
		static FName MuzzleSocket = FName(TEXT("MuzzleSocket"));
		
		//FVector StartPos = WeaponMesh->GetSocketLocation(MuzzleSocket).GetWorldLocation;
		//testing the raycast
		static FName WeaponFireTag = FName(TEXT("WeaponTrace"));
		

		auto HUD = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		HUD->Get3DCrossHairPos();
		// Start from the muzzle's position 
		FVector StartPos = WeaponMesh->GetSocketLocation(MuzzleSocket);
		// Get forward vector of MyPawn 
		FVector Forward = mOwner->GetActorForwardVector();
		// Calculate end position 
		FVector EndPos = HUD->CrossHair3DPos;//StartPos + Forward*WeaponRange;/*TODO: Figure out vector math */;

		
		// Perform trace to retrieve hit info 
		FCollisionQueryParams TraceParams(WeaponFireTag, true, Instigator);
		TraceParams.bTraceAsyncScene = true;
		TraceParams.bReturnPhysicalMaterial = true;
		
		// This fires the ray and checks against all objects w/ collision 
		FHitResult Hit(ForceInit);
		//FColor()
		
		DrawDebugLine(Instigator->GetWorld(), StartPos, EndPos, FColor::Red, true, 0.2f, 0.0f, 3.0f);
		GetWorld()->LineTraceSingleByObjectType(Hit, StartPos, EndPos, FCollisionObjectQueryParams::AllObjects, TraceParams);
		
		ATCube* cube = Cast<ATCube>(Hit.GetActor());

		
		if (Hit.bBlockingHit && cube)
		{ // TODO: Actually do something 
			//UGameplayStatics::SpawnEmitterAtLocation(this, ImpactEffect, Hit.ImpactPoint);
			//Debug_Log(i)
			if (cube)
			{
				if (cube->mParent)
				{
                    // The control mode is shoot to select tetromino
                    if (ATetrisManager::get()->IsShootToSelect())
                    {
                        if(ATetrisManager::get()->mSelectedTetromino != nullptr)
                        {
                            ATetrisManager::get()->mSelectedTetromino->Repaint();
                        }
                        ATetrisManager::get()->mSelectedTetromino = cube->mParent;
                        ATetrisManager::get()->mSelectedTetromino->Highlight();
                    }
                    // the control mode is shoot to move tetromino
                    else
                    {
                        switch (mProjType)
                        {
                            case 0:
                                if (mActionType == Projectile_Action::T_Rotation)
                                {
                                    cube->mParent->Rotate(Tetris_Direction::T_Forward);
                                }
                                else if (mActionType == Projectile_Action::T_Movement)
                                {
                                    cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Forward);
                                }
                                break;
                            case 1:
                                if (mActionType == Projectile_Action::T_Rotation)
                                {
                                    cube->mParent->Rotate(Tetris_Direction::T_Right);
                                }
                                else if (mActionType == Projectile_Action::T_Movement)
                                {
                                    cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Right);
                                }
                                break;
                            case 2:
                                if (mActionType == Projectile_Action::T_Rotation)
                                {
                                    cube->mParent->Rotate(Tetris_Direction::T_Backward);
                                }
                                else if (mActionType == Projectile_Action::T_Movement)
                                {
                                    cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Backward);
                                }
                                break;
                            default:
                                if (mActionType == Projectile_Action::T_Rotation)
                                {
                                    cube->mParent->Rotate(Tetris_Direction::T_Left);
                                }
                                else if (mActionType == Projectile_Action::T_Movement)
                                {
                                    cube->mParent->SingleMoveAtDirection(Tetris_Direction::T_Left);
                                }
                                break;
                        }
                    }
				}
			}
		}
		/*
		FVector MuzzleLocation = EndPos;//WeaponMesh->GetSocketLocation("MuzzleSocket");
		FRotator MuzzleRotation = FRotationMatrix::MakeFromX(MuzzleLocation - GetActorLocation()).Rotator();

		FVector MuzzleLocation = WeaponMesh->GetSocketLocation("MuzzleSocket");
		FRotator MuzzleRotation = FRotationMatrix::MakeFromX(MuzzleLocation-GetActorLocation()).Rotator();

		//MuzzleRotation.Pitch += 10.0f;          // skew the aim upwards a bit
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// spawn the projectile at the muzzle
			mProjectile = World->SpawnActor<AProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			//mProjectile->SetWeaponOwner(this);
			if (mProjectile)
			{
				mProjectile->SetWeaponOwner(this);
				// find launch direction
				if (mProjType == UP)
				{
					mProjectile->GetProjectileMesh()->SetMaterial(0, mProjectile->GetTheMaterial());
				}

				else if (mProjType == RIGHT)
				{
					mProjectile->GetProjectileMesh()->SetMaterial(0, mProjectile->GetTheRightMaterial());
				}

				else if (mProjType == DOWN)
				{
					mProjectile->GetProjectileMesh()->SetMaterial(0, mProjectile->GetTheDownMaterial());
				}

				else
				{
					mProjectile->GetProjectileMesh()->SetMaterial(0, mProjectile->GetTheLeftMaterial());
				}
                
                //Added by Rui Zeng
//                if (mActionType == Projectile_Action::T_Rotation) {
//                    Debug_Log("Fire Rotate Bullet")
//                }
//                else if(mActionType == Projectile_Action::T_Movement){
//                    Debug_Log("Fire Move Bullet")
//                }

                mProjectile->mActionType = mActionType;
                

				FVector const LaunchDir = MuzzleRotation.Vector();
				mProjectile->InitVelocity(LaunchDir);
			}
		}*/
	}
}

void AWeapon::OnStopFire()
{
	if (FireAC)
	{
		FireAC->Stop();
		FireAC = PlayWeaponSound(FireFinishSound);
		//FireAC = PlayWeaponSound(FireFinishSound);
		
	}
}

UAudioComponent* AWeapon::PlayWeaponSound(USoundCue* Sound)
{ UAudioComponent* AC = NULL; 
  if (Sound) 
  { AC = UGameplayStatics::SpawnSoundAttached(Sound, RootComponent); 
  } 
  return AC; 
}


void AWeapon::ToggleUp()
{
	mProjType = UP;
	Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mCurrentProjType = UP;

}

void AWeapon::ToggleRight()
{
	
	mProjType = RIGHT;
	Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mCurrentProjType = RIGHT;
}

void AWeapon::ToggleDown()
{
	
	mProjType = DOWN;
	Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mCurrentProjType = DOWN;
}

void AWeapon::ToggleLeft()
{
	mProjType = LEFT;
	Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD())->mCurrentProjType = LEFT;
}