// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Projectile.h"
#include "TetrisStatistics.h"
#include "TetrisClimberCharacter.h"
#include "Weapon.generated.h"


UCLASS()
class TETRISCLIMBER_API AWeapon : public AActor
{
	GENERATED_BODY()

	/*enum ProjectileType
	{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};
    
	ProjectileType mProjType = UP;
    */
	
public:	
	// Sets default values for this actor's properties
	enum ProjectileType
	{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};

	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USkeletalMeshComponent* GetWeaponMesh(){ return WeaponMesh;}

	void OnStopFire();
	void OnStartFire();

	void ToggleUp();
	void ToggleRight();
	void ToggleDown();
	void ToggleLeft();

	void SetOwner(ATetrisClimberCharacter* owner){mOwner = owner; }

	ProjectileType GetProjectileType(){ return mProjType;}

    void SetActionType(Projectile_Action type){mActionType = type;}
    
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Weapon) 
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, Category = Sound) 
	class USoundCue* FireLoopSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound) 
	class USoundCue* FireFinishSound;
	UPROPERTY(Transient) 
	class UAudioComponent* FireAC;
    
    Projectile_Action mActionType = Projectile_Action::T_Movement;


	/*UFUNCTION()
	void onFire();
	*/
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
    FVector MuzzleOffset;
	*/

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;


	UAudioComponent* PlayWeaponSound(USoundCue* Sound);
private:
	ATetrisClimberCharacter* mOwner;
	AProjectile* mProjectile;
	float WeaponRange = 10000.0f;
	ProjectileType mProjType = UP;
};
