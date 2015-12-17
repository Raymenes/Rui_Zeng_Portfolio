// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TetrisClimber.h"
#include "TetrisClimberCharacter.h"
#include "Weapon.h"
#include "TetrisClimberHUD.h"
#include "TetrisClimberGameMode.h"
#include "TetrisManager.h"

//////////////////////////////////////////////////////////////////////////
// ATetrisClimberCharacter

ATetrisClimberCharacter::ATetrisClimberCharacter()
{
	MyWeapon = nullptr;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//collision detection delegate...not sure which one works
	this->OnActorHit.AddDynamic(this, &ATetrisClimberCharacter::OnHit);
	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ATetrisClimber::OnHit);

	// Set size for collision capsule
	/*GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//first person camera
	// Create a CameraComponent 
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	//FirstPersonCamera->AttachParent = GetCapsuleComponent();
    FirstPersonCamera->AttachTo(GetMesh(), TEXT("hand_rSocket"));
    FirstPersonCamera->RelativeLocation = FVector(0, 0, 50.0f + BaseEyeHeight);
	FirstPersonCamera->bUsePawnControlRotation = false;*/

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    
    ////////trying to setup first person
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    
    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;
    
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;
    
    // Create a CameraComponent
    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->AttachParent = GetCapsuleComponent();
    FirstPersonCamera->RelativeLocation = FVector(0, 0, 64.0f); // Position the camera
    FirstPersonCamera->bUsePawnControlRotation = true;
    
    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->AttachParent = FirstPersonCamera;
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    
    
    
    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->AttachTo(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
    
    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

 
    Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
    Mesh3P->SetOnlyOwnerSee(true);
    Mesh3P->AttachParent = GetCapsuleComponent();
    Mesh3P->bCastDynamicShadow = false;
    Mesh3P->CastShadow = false;
    
    Mesh3P->ToggleVisibility();
    
    // Create a gun mesh component
    /*FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
    FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
    FP_Gun->bCastDynamicShadow = false;
    FP_Gun->CastShadow = false;
    FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);
    
    
    // Default offset from the character location for projectiles to spawn
    GunOffset = FVector(100.0f, 30.0f, 10.0f);*/
    
    // Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
    // derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    
    
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATetrisClimberCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// Set up gameplay key bindings
	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ATetrisClimberCharacter::TCharacterJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ATetrisClimberCharacter::TCharacterStopJumping);

	InputComponent->BindAxis("MoveForward", this, &ATetrisClimberCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATetrisClimberCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ATetrisClimberCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ATetrisClimberCharacter::LookUpAtRate);

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ATetrisClimberCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ATetrisClimberCharacter::TouchStopped);

	//for gun firing
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATetrisClimberCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ATetrisClimberCharacter::OnStopFire);
    
    InputComponent->BindAction("Rotate1", IE_Pressed, this, &ATetrisClimberCharacter::CounterClockwiseRotate);
    InputComponent->BindAction("Rotate2", IE_Pressed, this, &ATetrisClimberCharacter::ClockwiseRotate);
    
    InputComponent->BindAction("W", IE_Pressed, this, &ATetrisClimberCharacter::TMoveForward);
    InputComponent->BindAction("A", IE_Pressed, this, &ATetrisClimberCharacter::TMoveLeft);
    InputComponent->BindAction("S", IE_Pressed, this, &ATetrisClimberCharacter::TMoveBackward);
    InputComponent->BindAction("D", IE_Pressed, this, &ATetrisClimberCharacter::TMoveRight);
    
        //Added by Rui Zeng
    InputComponent->BindAction("Fire1", IE_Pressed, this, &ATetrisClimberCharacter::OnStartFireMoveBullet);
    InputComponent->BindAction("Fire1", IE_Released, this, &ATetrisClimberCharacter::OnStopFire);
    InputComponent->BindAction("Fire2", IE_Pressed, this, &ATetrisClimberCharacter::OnStartFireRotateBullet);
    InputComponent->BindAction("Fire2", IE_Released, this, &ATetrisClimberCharacter::OnStopFire);

	//switching between bullet types
	InputComponent->BindAction("ToggleLeft", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleLeft);
	InputComponent->BindAction("ToggleRight", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleRight);
	InputComponent->BindAction("ToggleUp", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleUp);
	InputComponent->BindAction("ToggleDown", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleDown);

	//switching cameras
	InputComponent->BindAction("ToggleFirstPerson", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleFirstPerson);
	InputComponent->BindAction("ToggleThirdPerson", IE_Pressed, this, &ATetrisClimberCharacter::OnToggleThirdPerson);

}


void ATetrisClimberCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void ATetrisClimberCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void ATetrisClimberCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ATetrisClimberCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ATetrisClimberCharacter::TCharacterJump()
{
    // Deselect Tetromino
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->Repaint();
        ATetrisManager::get()->mSelectedTetromino = nullptr;
    }
    else
    {
        Jump();
    }
    
}

void ATetrisClimberCharacter::TCharacterStopJumping()
{
    if(ATetrisManager::get()->mSelectedTetromino == nullptr)
    {
        StopJumping();
    }
}

void ATetrisClimberCharacter::ClockwiseRotate()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->ClockwiseRotate();
    }
}

void ATetrisClimberCharacter::CounterClockwiseRotate()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->CounterClockwiseRotate();

    }
}

void ATetrisClimberCharacter::TMoveForward()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->SingleMoveAtDirection(Tetris_Direction::T_Forward);
    }
}
void ATetrisClimberCharacter::TMoveBackward()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->SingleMoveAtDirection(Tetris_Direction::T_Backward);
    }
}
void ATetrisClimberCharacter::TMoveRight()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->SingleMoveAtDirection(Tetris_Direction::T_Right);
    }
}
void ATetrisClimberCharacter::TMoveLeft()
{
    if(ATetrisManager::get()->mSelectedTetromino != nullptr)
    {
        ATetrisManager::get()->mSelectedTetromino->SingleMoveAtDirection(Tetris_Direction::T_Left);
    }
}


void ATetrisClimberCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
        if(ATetrisManager::get()->mSelectedTetromino == nullptr)
        {
            // find out which way is forward
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            
            // get forward vector
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
            AddMovementInput(Direction, Value);
        }
	}
}

void ATetrisClimberCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
        if(ATetrisManager::get()->mSelectedTetromino == nullptr)
        {
            // find out which way is right
            const FRotator Rotation = Controller->GetControlRotation();
            const FRotator YawRotation(0, Rotation.Yaw, 0);
            
            // get right vector
            const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
            // add movement in that direction
            AddMovementInput(Direction, Value);
        }
	}
}


void ATetrisClimberCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Spawn the weapon, if one was specified 
	if (WeaponClass)
	{
		UWorld* World = GetWorld();

		if (World)
		{
            
            ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
            
            ATetrisManager::get()->InitializeHUD(hud);
            hud->SetPlayer(this);
            
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Need to set rotation like this because otherwise gun points down 
			FRotator Rotation(0.0f, 0.0f, 0.0f);
            FRotator Rotation3(0.0f, 90.0f, 0.0f);
			
			// Spawn the Weapon 
			MyWeapon = World->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, Rotation, SpawnParams);
            MyWeapon3 = World->SpawnActor<AWeapon>(WeaponClass, FVector::ZeroVector, Rotation3, SpawnParams);
			if (MyWeapon)
			{
				// This is attached to "WeaponPoint" which is defined in the skeleton
				//MyWeapon->SetMyOwner(this);
				MyWeapon->GetWeaponMesh()->AttachTo(Mesh1P, TEXT("GripPoint"));//("WeaponPoint"));
				MyWeapon->SetOwner(this);
                
                MyWeapon3->GetWeaponMesh()->AttachTo(Mesh3P, TEXT("hand_rSocket"));
                MyWeapon3->GetWeaponMesh()->ToggleVisibility();
                
			}
		}
	}
}

void ATetrisClimberCharacter::OnStopFire()
{
	if (MyWeapon && mFirstPersonView)
	{
		MyWeapon->OnStopFire();
	}
}
//Added by Rui Zeng
void ATetrisClimberCharacter::OnStartFireMoveBullet()
{
    if (MyWeapon && mFirstPersonView)
    {
        MyWeapon->SetActionType(Projectile_Action::T_Movement);
        OnStartFire();
    }
}
//Added by Rui Zeng
void ATetrisClimberCharacter::OnStartFireRotateBullet()
{
    if (MyWeapon && mFirstPersonView)
    {
        MyWeapon->SetActionType(Projectile_Action::T_Rotation);
        OnStartFire();
    }
}


void ATetrisClimberCharacter::OnStartFire()
{
	if (MyWeapon && mFirstPersonView)
	{
		MyWeapon->OnStartFire();
	}
}

void ATetrisClimberCharacter::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit){
    
    
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
    
    
    /*if(Hit.bBlockingHit){
    
        GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
    }*/
    
}

void ATetrisClimberCharacter::NotifyActorBeginOverlap(AActor* OtherActor){
    
    Super::NotifyActorBeginOverlap(OtherActor);
    
    FTimerHandle timer;
    GetWorldTimerManager().SetTimer(timer, this, &ATetrisClimberCharacter::ResetLocation, 1.0f);
    
    
    
}


void ATetrisClimberCharacter::ResetLocation(){
    
    FVector newLocation = FVector(-810.0f, 280.0f, 226.278412f);
    //SetActorLocation(newLocation);
    
}

void ATetrisClimberCharacter::OnToggleUp()
{
	MyWeapon->ToggleUp();
}

void ATetrisClimberCharacter::OnToggleRight()
{
	MyWeapon->ToggleRight();
}

void ATetrisClimberCharacter::OnToggleDown()
{
	MyWeapon->ToggleDown();
}

void ATetrisClimberCharacter::OnToggleLeft()
{
	MyWeapon->ToggleLeft();
}


void ATetrisClimberCharacter::OnToggleFirstPerson()
{
	
	if (!mFirstPersonView)
	{
			
		FirstPersonCamera->Activate();
		FollowCamera->Deactivate();
		mFirstPersonView = true;
        Mesh1P->ToggleVisibility();
        Mesh3P->ToggleVisibility();
        MyWeapon->GetWeaponMesh()->ToggleVisibility();
        MyWeapon3->GetWeaponMesh()->ToggleVisibility();
        bUseControllerRotationPitch = true;
        bUseControllerRotationYaw = true;
        bUseControllerRotationRoll = true;
        ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
        hud->ShowHUD();

	}
}

void  ATetrisClimberCharacter::OnToggleThirdPerson()
{
	
	if (mFirstPersonView)
	{
		
		FollowCamera->Activate();
		FirstPersonCamera->Deactivate();
		mFirstPersonView = false;
        Mesh3P->ToggleVisibility();
        Mesh1P->ToggleVisibility();
        MyWeapon->GetWeaponMesh()->ToggleVisibility();
        MyWeapon3->GetWeaponMesh()->ToggleVisibility();
        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;
        ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
        hud->ShowHUD();

	
	}
}

void ATetrisClimberCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//check if we've won ie escaped the room
	/*if (GetActorLocation().Z >= 3800)//3800 is height of opening
	{
		ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
		if (!hud->mGameOver)
		{
			hud->mGameOver = true;
		}
	}*/

}

void ATetrisClimberCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Hit.bBlockingHit)
	{
		ATCube* cube = Cast<ATCube>(OtherActor);
		if (cube)
		{
			if (cube->mParent)
			{
				if (cube->GetActorLocation().Z > GetActorLocation().Z)
				{
					SetIsAlive(false);
					ATetrisClimberHUD* hud = Cast<ATetrisClimberHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
					hud->mGameOver = true;
				}
			}
		}
	}
}