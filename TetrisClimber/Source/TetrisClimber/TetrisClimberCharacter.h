// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "TetrisClimberCharacter.generated.h"



UCLASS(config=Game)
class ATetrisClimberCharacter : public ACharacter
{
	GENERATED_BODY()
    
    UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    class USkeletalMeshComponent* Mesh1P;

    
    UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    class USkeletalMeshComponent* Mesh3P;
    
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	class AWeapon* MyWeapon;

    class AWeapon* MyWeapon3;

	bool mFirstPersonView = true;

	bool mAlive = true;

public:
	ATetrisClimberCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
    
    void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
    
    void NotifyActorBeginOverlap(AActor* OtherActor) override;


	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void OnStopFire();
	void OnStartFire();

	void OnToggleUp();
	void OnToggleRight();
	void OnToggleDown();
	void OnToggleLeft();

    void ResetLocation();
    
    
	void OnToggleFirstPerson();
	void OnToggleThirdPerson();

	bool IsAlive(){ return mAlive;}
	void SetIsAlive(bool alive){ mAlive = alive;}

	
protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
    
    void OnStartFireMoveBullet();
    
    void OnStartFireRotateBullet();
    
    void TCharacterJump();
    
    void TCharacterStopJumping();
    
    void ClockwiseRotate();
    
    void CounterClockwiseRotate();
    
    void TMoveForward();
    void TMoveBackward();
    void TMoveRight();
    void TMoveLeft();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UFUNCTION()
	void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

public:
    /** Returns Mesh1P subobject **/
    FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
    
    /** Returns Mesh1P subobject **/
    FORCEINLINE class USkeletalMeshComponent* GetMesh3P() const { return Mesh3P; }
    
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }
};

