// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"

#include "TetrisStatistics.h"
#include "Tetromino.h"
#include "TCube.h"
#include "Projectile.generated.h"


UCLASS()
class TETRISCLIMBER_API AProjectile : public AActor
{
	GENERATED_BODY()
    	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	AProjectile(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void InitVelocity(const FVector& ShootDirection);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
	UProjectileMovementComponent* ProjectileMovement;
	/*
	void ToggleUp();
	void ToggleRight();
	void ToggleDown();
	void ToggleLeft();
	*/
	

	void SetWeaponOwner(class AWeapon* owner){ mOwner = owner; }

	UStaticMeshComponent* GetProjectileMesh(){ return ProjectileMesh;}

	UMaterialInstance* GetTheMaterial(){ return TheMaterial;}
	UMaterialInstance* GetTheRightMaterial(){ return TheRightMaterial;}
	UMaterialInstance* GetTheDownMaterial(){ return TheDownMaterial;}
	UMaterialInstance* GetTheLeftMaterial(){ return TheLeftMaterial;}
    
	UFUNCTION()
	void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    
    Projectile_Action mActionType = Projectile_Action::T_Rotation;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;	
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;

	class AWeapon* mOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UMaterialInstance* TheMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UMaterialInstance* TheRightMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UMaterialInstance* TheDownMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	UMaterialInstance* TheLeftMaterial;

	
    bool canAct = true;
};
