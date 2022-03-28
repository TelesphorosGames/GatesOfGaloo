// Fill out your copyright notice in the Description page of Project Settings.


#include "MainChar.h"
#include "AIController.h"
#include "Enemy.h"
#include "FirstSaveGame.h"
#include "ItemStorage.h"
#include "MainAnimInstance.h"
#include "MainPlayerController.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/NavMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"


	

// EPIC: // Sets default values
AMainChar::AMainChar()
{
 	// EPIC: // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*
	**Camera Initialization 

	**Main Camera Is Called Follow Camera, and is attached to the Camera Boom : 

	// Initializes the Camera Boom as a Unreal Object called Camera Boom, of type USpringArmComponent
	// Sets up Camera Boom to Root Component of Main Character (the USceneComponent Default Root)
	// Sets the Camera Boom Length to ~6 meters
	// Sets the Camera Boom rotation to match the rotation of the Player's Pawn by default
	// Create two more UCameraComponents and set their initial attachments and base values as well : ( One for third person and one for first person ) :
	// Sets base turn rate values for Follow camera 
	// Don't rotate when the controller rotates, only affect the camera.

	*/

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; 
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	BaseTurnRate= 65.f;
	BaseLookUpRate = 65.f;

	/*
	***May Come In Handy One Day : 
	// LockedOnCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("LockOnCameraBoom"));
	// LockedOnCameraBoom->SetupAttachment(GetRootComponent());
	// LockedOnCameraBoom->bUsePawnControlRotation = true;
	// LockedOnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("LockedOnCamera"));
	// LockedOnCamera->SetupAttachment(LockedOnCameraBoom, USpringArmComponent::SocketName);

	*/

	//*//*//* Character Movement
	//*//*//*

	/*
	// Character moves in the direction of input
	// Affects the character movement Rate of rotation.
	// Sets character's jump velocity.
	// Control of character while in air

	// GetCharacterMovement()->bUseControllerDesiredRotation = true; // We can use this line of code if we want the character's direction to be locked facing forward (or facing the direction of the camera to be more specific.)
	*/

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.3f;
	GetCharacterMovement()->RotationRate = FRotator {0.f, 540.f, 0.f};
	// Setting the initial movement Enum variables to their defaults:
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	InterpToEnemySpeed = 15.f;

}

void AMainChar::BeginPlay()
// Called when the game starts or when spawned
{
	Super::BeginPlay();

	Health = MaxHealth;
	Mana = MaxMana;
	Stamina = MaxStamina;

	//* Sets initial combat booleans to false
	bAttacking = false;
	bIsBowEquipped =false;
	bMainHandEquipped = false;
	bOffHandEquipped = false;
	bLMBDown = false;

	SwordSocket = GetMesh()->GetSocketByName("LeftShoulderSocket");
	DaggerSocket = GetMesh()->GetSocketByName("DaggerSocket");
	LeftHandSocket = GetMesh()->GetSocketByName("LeftHandSocket");
    RightHandSocket = GetMesh()->GetSocketByName("RightHandSocket");

	GetCharacterMovement()->MaxWalkSpeed = RunSpeedVar;
	
	FirstPersonCamera->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "HeadSocket");

	MainPlayerController = Cast<AMainPlayerController>(GetController());

	const FInputModeGameOnly Gmo;
	MainPlayerController->SetInputMode(Gmo);
	

}

// Called every frame
void AMainChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch(StaminaStatus)
	{
		case EStaminaStatus::ESS_Normal:
		if(bSprinting)
		{
			if(Stamina-DeltaStamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
				Stamina -= DeltaStamina;
			}
			else
			{
				Stamina -= DeltaStamina;
			}
		}
		else if(!bSprinting)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Recovering);
		}
		break;

		case EStaminaStatus::ESS_BelowMinimum:
		if(bSprinting)
		{
			Stamina -= (DeltaStamina/2);
			if(Stamina <= 0.1f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Drained);
				Stamina = 0.f;
			}
		}
		else
		{
			Stamina += (DeltaStamina/2);
			if(!bSprinting && Stamina >= MinSprintStamina)
			{
			SetStaminaStatus(EStaminaStatus::ESS_Recovering);
			}
		
		}
		break;

		case EStaminaStatus::ESS_Drained:
		{
			SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
		}

		break;

		case EStaminaStatus::ESS_Recovering:

			if(!bSprinting && !bAttacking)
			{
				if(Stamina + DeltaStamina >= MaxStamina)
				{
					Stamina = MaxStamina;
				}
				else
				{
					Stamina += DeltaStamina;
				}
			}
			else
			if(Stamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			else
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}

		break;

		case EStaminaStatus::ESS_MAX:
		;
	}

	GetOverlappingActors(AllOverlappingEnemies, EnemyFilter);
	GetLockOnPotentialTargets(AllOverlappingEnemies, LockOnPotentialTargets);

	if(LockOnPotentialTargets.Num() > 0)
	{
		bEnemyInRange = true;
		if(CombatTarget)
		{
			CombatTargetLocation = CombatTarget->GetActorLocation();
			if(MainPlayerController)
			{
				MainPlayerController->EnemyLocation = CombatTargetLocation;
			}
			if(CombatTarget && CombatTarget->IsPendingKill() && bEnemyInRange == true || CombatTarget->Alive() == false)
			{
				CombatTarget=GetClosestEnemy();
			}
			else if (CombatTarget && CombatTarget ->IsPendingKill() && bEnemyInRange ==false)
			{
				CombatTarget = nullptr;
				bHasCombatTarget=false;
				return;
			}
	
			if(bLockedOnPerspective)
			{
				if(CombatTarget && bInterpToEnemy)
				{
					CurrentLookAtYawTarget = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
					const FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), CurrentLookAtYawTarget, DeltaTime, InterpToEnemySpeed);
					SetActorRotation(InterpRotation);
				}
				if(LockedOnEnemy && LockedOnEnemy->Alive())
				{
					const FRotator CurrentCamSpot = GetControlRotation();
					const FRotator FaceEnemy = UKismetMathLibrary::FindLookAtRotation(
						GetActorLocation(), LockedOnEnemy->GetActorLocation());
					const FRotator InterpBetween = FMath::RInterpTo(CurrentCamSpot, FaceEnemy, DeltaTime, 2.f);
					Controller->SetControlRotation(InterpBetween);
					MainPlayerController->DisplayEnemyHealthBar();
				}
				else
				{
					CycleTarget();
					LockedOnEnemy = CombatTarget;
				}
			}
		}
		LockedOnEnemy=CombatTarget;
	} 

	if(CombatTarget==nullptr)
	{
		MainPlayerController->RemoveEnemyHealthBar();
	}
}


FRotator AMainChar::GetLookAtRotationYaw(const FVector Target) const
{
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	const FRotator LookAtRotationYaw = {0.f, LookAtRotation.Yaw, 0.f};
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AMainChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Sends a check command to verify that the PlayerInput Compnent is valid
	check(PlayerInputComponent);

	//Binds all of our Input functions : 
	//Jump is an action input that requires an input ENUM, the ones used here are IE_pressed and IE_released. 

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainChar::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainChar::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainChar::MyCrouch);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMainChar::MyRun);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainChar::MySprint);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainChar::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainChar::LMBUp);

	PlayerInputComponent->BindAction("TogglePause", IE_Pressed, this, &AMainChar::TogglePause);

	PlayerInputComponent->BindAction("CameraUp", IE_Pressed, this, &AMainChar::AdjustCameraLengthDown);
	PlayerInputComponent->BindAction("CameraDown", IE_Pressed, this, &AMainChar::AdjustCameraLengthUp);

	PlayerInputComponent->BindAction("AdjustPerspective", IE_Pressed, this, &AMainChar::AdjustPerspective);
	PlayerInputComponent->BindAction("LockOnPerspective", IE_Pressed, this, &AMainChar::LockOnPerspective);
	PlayerInputComponent->BindAction("CycleTarget", IE_Pressed, this, &AMainChar::CycleTarget);

	PlayerInputComponent->BindAction("EquipBow", IE_Pressed, this, &AMainChar::EquipBow);
	PlayerInputComponent->BindAction("EquipSword", IE_Pressed, this, &AMainChar::EquipSword);
	PlayerInputComponent->BindAction("DisarmSword", IE_Pressed, this, &AMainChar::DisarmSword);

	PlayerInputComponent->BindAction("AttackOne", IE_Pressed, this, &AMainChar::AttackOne);
	PlayerInputComponent->BindAction("AttackTwo", IE_Pressed, this, &AMainChar::AttackTwo);
	PlayerInputComponent->BindAction("AttackThree", IE_Pressed, this, &AMainChar::AttackThree);
	PlayerInputComponent->BindAction("AttackFour", IE_Pressed, this, &AMainChar::AttackFour);
	PlayerInputComponent->BindAction("AttackFive", IE_Pressed, this, &AMainChar::AttackFive);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainChar::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMainChar::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMainChar::LookUpAtRate);
}


void AMainChar::MoveForward(float Value)
{
	// || (bMidAnim))|| (bAttacking)
	if((Controller == nullptr)  || (MovementStatus == EMovementStatus::EMS_Dead) ) return;
	{
		if(!bSprinting || (bSprinting && Value >= 0.1f))
		{
			//Finding which way is Forward to the player's Controller
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation = {0, Rotation.Yaw, 0};
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			AddMovementInput(Direction, Value);
		}
		if(bSprinting && (Value <= 0.1f) || bSprinting && StaminaStatus == EStaminaStatus::ESS_Drained)
		{
			if (!bRunning)
			{
				GetCharacterMovement()->MaxWalkSpeed = 125.f;
			}
			else
			{
				GetCharacterMovement()->MaxWalkSpeed = RunSpeedVar;
			} 
			bSprinting=false;	
		}
	}
}

void AMainChar::MoveRight(float Value)
{

	if( (MovementStatus == EMovementStatus::EMS_Dead) ){return;}
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation = {0, Rotation.Yaw, 0};

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
}

void AMainChar::TurnAtRate(const float Rate)
{
	if(MovementStatus != EMovementStatus::EMS_Dead)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMainChar::LookUpAtRate(const float Rate)
{
	if(MovementStatus == EMovementStatus::EMS_Dead){return;}
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
 
void AMainChar::MyCrouch()
{
	if(MovementStatus == EMovementStatus::EMS_Dead){return;}
	if(GetMovementComponent()->IsCrouching())
	{
		ACharacter::UnCrouch();
		bMyCrouchMain = false;
	}
	else
	{
		ACharacter::Crouch();
		bMyCrouchMain = true;
	}
}

void AMainChar::MyRun()
{
	if(bSprinting || (MovementStatus == EMovementStatus::EMS_Dead)){return;}
	if (!bRunning)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeedVar;
		bRunning= true;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		bRunning = false;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst Can't be const if bound
void AMainChar::AdjustCameraLengthUp()
{
	if(CameraBoom->TargetArmLength <= 00.f)
	{
		return;
	}
	else
	{
		CameraBoom->TargetArmLength -=30.f;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst Can't be const if bound
void AMainChar::AdjustCameraLengthDown()
{
	if(CameraBoom->TargetArmLength >= 1400.f)
	{
		return;
	}
	else
	{
		CameraBoom->TargetArmLength +=30.f;
	}
}

void AMainChar::MySprint()
{
	if(MovementStatus == EMovementStatus::EMS_Dead || bIsSwordEquipped == true){return;}
	if(bLockedOnPerspective)
	{
		bLockedOnPerspective = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	if (!bSprinting && StaminaStatus != EStaminaStatus::ESS_Drained && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
					
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeedVar;
		bSprinting= true;
	}
	else
	{
		if(!bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = 125.f;
			bSprinting = false;
		}
		if(bRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeedVar;
			bSprinting = false;
		}
	}	
}

void AMainChar::Jump()
{
	if(bMyCrouchMain)
	{
		MyCrouch();
	}
	Super::Jump();
	bJustJumped = true;
}

void AMainChar::StopJumping()
{
	Super::StopJumping();
	
	bJustJumped=false;
}


void AMainChar::EquipBow()
{
	if(Controller != nullptr)
	{
		if (bIsBowEquipped)
		{
			DisarmBow();
			bMidAnim=true;
			return;
		}
		else
		{
			bMidAnim=true;
			bIsBowEquipped = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, 1.f );
				AnimInstance->Montage_JumpToSection(FName("EquipBow"), DaggerCombatMontage);
			}
		}
	}
}

void AMainChar::DisarmBow()
{
	if(Controller != nullptr)
	{
		if (!bIsBowEquipped)
		{
			EquipBow();
			bMidAnim=true;
			return;
		}
		bMidAnim=true;
		bIsBowEquipped = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && DaggerCombatMontage)
		{
			AnimInstance->Montage_Play(DaggerCombatMontage, 1.f );
			AnimInstance->Montage_JumpToSection(FName("DisarmBow"), DaggerCombatMontage);
		}
	}
}

void AMainChar::EquipSword()
{
	if(MovementStatus == EMovementStatus::EMS_Dead || (bSprinting)) return;
	
	if(bIsSwordEquipped && EquippedWeapon)
	{
		DisarmSword();
		bMidAnim=true;
		return;
	}
	else if(EquippedWeapon)
	{
		bIsSwordEquipped = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && DaggerCombatMontage)
		{
			AnimInstance->Montage_Play(DaggerCombatMontage, 1.f );
			AnimInstance->Montage_JumpToSection(FName("EquipSword"), DaggerCombatMontage);
	  		bMidAnim=true;
		}
	}
}

void AMainChar::DisarmSword()
{
	if(MovementStatus == EMovementStatus::EMS_Dead || (bSprinting)) return;
	{
		if(bIsSwordEquipped == false && (EquippedWeapon))
		{
			EquipSword();
			return;
		}
		else if(EquippedWeapon)
		{
			bIsSwordEquipped = false;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, 1.f );
				AnimInstance->Montage_JumpToSection(FName("DisarmSword"), DaggerCombatMontage);
				bMidAnim=true;
			}

		}
	}
}

void AMainChar::AttachSwordToBack()
{
	if(SwordSocket && EquippedWeapon)
		{
			SwordSocket->AttachActor(GetEquippedWeapon(), GetMesh());
		}
	bMidAnim=false;
	bAttacking = false;
}

void AMainChar::DetachSwordFromBack()
{
	if(RightHandSocket && EquippedWeapon)
	{
		RightHandSocket->AttachActor(GetEquippedWeapon(), GetMesh());
	}
	bMidAnim=false;
	bAttacking = false;
}

void AMainChar::AttachDaggerToWaist()
{
	if(LeftHandSocket && EquippedOffHand)
	{
		LeftHandSocket->AttachActor(GetEquippedOffHand(), GetMesh());
	}
	bMidAnim=false;
	bAttacking = false;
}

void AMainChar::DetachDaggerFromWaist()
{
	if(DaggerSocket && EquippedOffHand)
	{
		DaggerSocket->AttachActor(GetEquippedOffHand(), GetMesh());
	}
	bMidAnim=false;
	bAttacking = false;
}



void AMainChar::AdjustPerspective()
{
	if(MovementStatus == EMovementStatus::EMS_Dead)
	{
		GetFirstPersonCamera()->SetActive(false, true);
		GetFollowCamera()->SetActive(true, true);	
		return;
	}
	bLockedOnPerspective = false;

	const FVector CameraLoc = FirstPersonCamera->GetComponentLocation();
	if(!bFPPerspective)
	{
		GetCharacterMovement()->bUseControllerDesiredRotation=true;
		GetCharacterMovement()->bOrientRotationToMovement=false;
		GetFirstPersonCamera()->SetActive(true, true);
		GetFollowCamera()->SetActive(false, true);
		bFPPerspective = true;
		
	}
	else
	{
		GetFirstPersonCamera()->SetActive(false, true);
		GetFollowCamera()->SetActive(true, true);	
		GetCharacterMovement()->bOrientRotationToMovement=true;
		bFPPerspective = false;
		FirstPersonCamera->SetWorldLocation(CameraLoc);
	}
}

void AMainChar::LockOnPerspective()
{
	
	if(bSprinting) return;
	if(MovementStatus == EMovementStatus::EMS_Dead)
	{
		bLockedOnPerspective = false;
		bCurrentlyLocked = false;
		CombatTarget = nullptr;
		MainPlayerController->RemoveEnemyHealthBar();

		return;
	}
	if(!bLockedOnPerspective)
	{
		if(!LockedOnEnemy && LockOnPotentialTargets.Num() >= 1)
		{
			UE_LOG(LogTemp,Warning,TEXT("Lock on targets > 0 ! "));
			LockedOnEnemy=GetClosestEnemy();

			if(LockedOnEnemy && LockedOnEnemy->Alive())
			{
				CombatTarget=LockedOnEnemy;
				bHasCombatTarget = true;
				MainPlayerController->DisplayEnemyHealthBar();	
			}
		}
		else if(LockedOnEnemy && LockedOnEnemy->bOverlappingChar == false)
		{
			LockedOnEnemy = GetClosestEnemy();
			CombatTarget = LockedOnEnemy;
		}
		else
		{
			LockedOnEnemy = CombatTarget;
		}
		GetCharacterMovement()->bOrientRotationToMovement = false;
		bLockedOnPerspective = true;
		bCurrentlyLocked=true;
	}
	else if(bLockedOnPerspective)
	{
		if(!bCurrentlyLocked)
		{
			LockedOnEnemy=GetClosestEnemy();
			if(LockedOnEnemy && LockedOnEnemy->Alive())
			{
				bCurrentlyLocked=true;
				CombatTarget=LockedOnEnemy;
				bHasCombatTarget = true;
				MainPlayerController->DisplayEnemyHealthBar();
				bLockedOnPerspective = true;
				return;
			}
		}
		else if(bCurrentlyLocked)
		{
			bCurrentlyLocked=false;
				CombatTarget=nullptr;
				LockedOnEnemy=nullptr;
				bHasCombatTarget = false;
				MainPlayerController->RemoveEnemyHealthBar();
				bLockedOnPerspective = false;
				GetCharacterMovement()->bOrientRotationToMovement = true;
				return;
		}
		if(bFPPerspective)
		{
			GetCharacterMovement()->bOrientRotationToMovement=false;
			
		}
		else
		{
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		bLockedOnPerspective = false;
		bCurrentlyLocked = false;
	}
}

AEnemy* AMainChar::GetClosestEnemy()
{
	if(LockOnPotentialTargets.Num()>=1)
	{
		AEnemy* ClosestEnemy = Cast<AEnemy>(LockOnPotentialTargets[0]);
		if(ClosestEnemy && (ClosestEnemy->Alive()))
		{
			float MinDistance = GetDistanceTo(ClosestEnemy);
			for(AActor* Enemies : LockOnPotentialTargets)
			{
				const float DistanceTo = GetDistanceTo(Enemies);
				if (DistanceTo < MinDistance)
				{
					MinDistance = DistanceTo;
					AEnemy* NewClosestEnemy = Cast<AEnemy>(Enemies);
					ClosestEnemy = NewClosestEnemy;
				}
					
			}
			return ClosestEnemy;
		}
		else  
		{
			return nullptr;
		}
	}	
	else
	{
		return nullptr;
	}
}

float AMainChar::GetHealthPercentage() const
{
	const float HealthPercentage = Health / MaxHealth;
	return HealthPercentage;
}

float AMainChar::GetManaPercentage() const
{
	const float ManaPercentage = Mana / MaxMana;
	return ManaPercentage;
}

float AMainChar::GetStaminaPercentage() const
{
	const float StaminaPercentage = Stamina / MaxStamina;
	return StaminaPercentage;
}

void AMainChar::IncreaseCoins(int32 Coin)
{
	Coins+=Coin;
}

void AMainChar::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;
}

void AMainChar::LMBDown()
{
	bLMBDown = true;

	if(ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if(Weapon)
		{
			Weapon->Equip(this);
			SetAOI(nullptr);
		}
	}
	else if(EquippedWeapon || EquippedOffHand)
	{
		AttackOne();
	}

}

void AMainChar::LMBUp()
{
	bLMBDown = false;
}

void AMainChar::SetEquippedWeapon(AWeapon* WepToSet)
{
	if(EquippedWeapon || WepToSet->WeaponType == EWeaponType::EWT_Bow)
	{
		SetEquippedOffHand(WepToSet);
	}
	else
	{
	EquippedWeapon = WepToSet;
	bMainHandEquipped = true;
	}
}

AWeapon* AMainChar::GetEquippedWeapon() const
{
	return EquippedWeapon;
}

void AMainChar::SetEquippedOffHand(AWeapon* WepToSet)
{
	EquippedOffHand = WepToSet;
	bOffHandEquipped = true;
	UE_LOG(LogTemp,Warning, TEXT("Set Equipped Off Hand Called"));
}

AWeapon* AMainChar::GetEquippedOffHand() const
{
	return EquippedOffHand;
}

void AMainChar::Attack()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			bAttacking = true;
			
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && SwordCombatMontage)
			{
				const int Section = FMath::RandRange(0,4);
				switch (Section)
				{
					case 0:
						FirstPersonCamera->bUsePawnControlRotation = false;
						AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_1"), SwordCombatMontage);
						Stamina -= 20;

						break;
					case 1:
						FirstPersonCamera->bUsePawnControlRotation = false;
						AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_2"), SwordCombatMontage);
						Stamina -= 15;
						break;
					case 2:
						FirstPersonCamera->bUsePawnControlRotation = false;
						AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_3"), SwordCombatMontage);
						Stamina -= 10;
						break;
					case 3:
						AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_4"), SwordCombatMontage);
						Stamina -= 5;
						break;
					case 4:
						AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_5"), SwordCombatMontage);
						Stamina -= 25;
						break;

					default:
					;
				}
			}	
		}	
		else if(bOffHandEquipped && !bMainHandEquipped)
		{
				
			bAttacking = true;
			
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				const int Section = FMath::RandRange(0,4);
				switch (Section)
				{
					case 0:
						AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_1"), DaggerCombatMontage);
						Stamina -= 20;
						break;
					case 1:
						AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_2"), DaggerCombatMontage);
						Stamina -= 10;
						break;
					case 2:
						AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_3"), DaggerCombatMontage);
						Stamina -= 25;
						break;
					case 3:
						AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_4"), DaggerCombatMontage);
						Stamina -= 15;
						break;
					case 4:
						AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
						AnimInstance->Montage_JumpToSection(FName("Attack_5"), DaggerCombatMontage);
						Stamina -= 5;
						break;

					default:
					;

				}
			}
		}		
		else if(bMainHandEquipped && bOffHandEquipped)
		{
			
			bAttacking = true;
			Stamina -= 15;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				const int Section = FMath::RandRange(0,4);
				switch (Section)
				{
				case 0:
					AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_1"), DualWieldMontage);
					Stamina -= 25;
					break;
				case 1:
					FirstPersonCamera->bUsePawnControlRotation = false;
					AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_2"), DualWieldMontage);
					Stamina -= 20;
					break;
				case 2:
					AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_3"), DualWieldMontage);
					Stamina -= 10;
					break;
				case 3:
					FirstPersonCamera->bUsePawnControlRotation = false;
					AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_4"), DualWieldMontage);
					Stamina -= 15;
					break;
				case 4:
					AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_5"), DualWieldMontage);
					Stamina -= 5;
					break;

				default:
					;

				}
			}
		}
	}
}


void AMainChar::StopAttack()
{
	bAttacking = false;
	FirstPersonCamera->bUsePawnControlRotation = true;
	SetInterpToEnemy(false);

	if(bLMBDown)
	{
		
		AttackOne();
	}

}

void AMainChar::AttackOne()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum &&(!bMidAnim))
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			if(bIsSwordEquipped == false && (EquippedWeapon))
			{
				DisarmSword();
				return;
			}
			else
			{
				bAttacking = true;
				SetInterpToEnemy(true);
				// FirstPersonCamera->bUsePawnControlRotation = false;
				Stamina -= 5;
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if(AnimInstance && SwordCombatMontage)
				{
					AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
					AnimInstance->Montage_JumpToSection(FName("Attack_4"), SwordCombatMontage);
				}

			}
			
		}
	
		else if(bOffHandEquipped && !bMainHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 5;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_5"), DaggerCombatMontage);
			}
		}

		else if(bMainHandEquipped && bOffHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 5;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_5"), DualWieldMontage);
			}
		}
	}
}

void AMainChar::AttackTwo()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			bAttacking = true;
			FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 10;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && SwordCombatMontage)
			{
				AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_3"), SwordCombatMontage);
			}
		}

		
		else if(bOffHandEquipped && !bMainHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 10;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), DaggerCombatMontage);
			}
		}

			else if(bMainHandEquipped && bOffHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 10;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_3"), DualWieldMontage);
			}
		}
	}
}

void AMainChar::AttackThree()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			bAttacking = true;
			FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 15;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && SwordCombatMontage)
			{
				AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), SwordCombatMontage);
			}
		}

		
		else if(bOffHandEquipped && !bMainHandEquipped)
		{
			bAttacking = true;
			
			Stamina -= 15;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_4"), DaggerCombatMontage);
			}
		}

			else if(bMainHandEquipped && bOffHandEquipped)
		{
			bAttacking = true;
			FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 15;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_4"), DualWieldMontage);
			}
		}

	}
}

void AMainChar::AttackFour()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			bAttacking = true;
			FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 20;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && SwordCombatMontage)
			{
				AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), SwordCombatMontage);
			} 
		}

		else if(bOffHandEquipped && !bMainHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 20;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), DaggerCombatMontage);
			}
		}

			else if(bMainHandEquipped && bOffHandEquipped)
		{
			bAttacking = true;
			FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 20;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_2"), DualWieldMontage);
			}
		}
	}
}

void AMainChar::AttackFive()
{
	if(!bAttacking && StaminaStatus != EStaminaStatus::ESS_BelowMinimum)
	{
		if(bMainHandEquipped && !bOffHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 25;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && SwordCombatMontage)
			{
				AnimInstance->Montage_Play(SwordCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_5"), SwordCombatMontage);
			}
		}

		
		else if(bOffHandEquipped && !bMainHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 25;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DaggerCombatMontage)
			{
				AnimInstance->Montage_Play(DaggerCombatMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_3"), DaggerCombatMontage);
			}
		}

			else if(bMainHandEquipped && bOffHandEquipped)
		{
			bAttacking = true;
			// FirstPersonCamera->bUsePawnControlRotation = false;
			Stamina -= 25;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance && DualWieldMontage)
			{
				AnimInstance->Montage_Play(DualWieldMontage, AttackSpeed);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), DualWieldMontage);
			}
		}
	}
}

void AMainChar::PlaySwingSound()
{
	if(EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

void AMainChar::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

void AMainChar::IncreaseHealth(float Amount)
{
	if((Health+Amount) > MaxHealth)
	{
		Health = MaxHealth;
	}
	else
	{
		Health += Amount;
	}
}

void AMainChar::DecreaseHealth(float Amount)
{	
	if (Health - Amount <= 0.f)
	{
		Health -= Amount;
		NoMoreHealth();
	}
	else
	{
		Health-=Amount;
	}
}

float AMainChar::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	DecreaseHealth(DamageAmount);

	return DamageAmount;
}

void AMainChar::NoMoreHealth()
{
	if(MovementStatus != EMovementStatus::EMS_Dead)
	{
		MainPlayerController->RemoveEnemyHealthBar();
		bIsValidTarget = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		InputComponent->RemoveActionBinding(FName("Jump"), EInputEvent::IE_Pressed);
		SetMovementStatus(EMovementStatus::EMS_Dead);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && SwordCombatMontage)
		{
			AnimInstance->Montage_Play(SwordCombatMontage);
			AnimInstance->Montage_JumpToSection(FName("Death"));
		}
	}
}

void AMainChar::DeathEnd()
{
	GetMesh()-> bPauseAnims = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainChar::CycleTarget()
{
	if(!CombatTarget && LockOnPotentialTargets.Num() >= 1)
	{
		CombatTarget = GetClosestEnemy();
		bHasCombatTarget=true;
		LockedOnEnemy=CombatTarget;
		bCurrentlyLocked=true;
		return;
	}

	if(LockOnPotentialTargets.Num() == 0 || MovementStatus == EMovementStatus::EMS_Dead)
	{
		bEnemyInRange=false;
		bHasCombatTarget=false;
		CombatTarget=nullptr;
		LockedOnEnemy=nullptr;
		bCurrentlyLocked=false;
		return;
	}
	
	if(LockOnPotentialTargets.Num() == 1) 
		{
			NextLockedEnemy=Cast<AEnemy>(LockOnPotentialTargets[0]);
			if(NextLockedEnemy->Alive())
			{
				CombatTarget = NextLockedEnemy;
				LockedOnEnemy=CombatTarget;
			}
			else
			{
				CombatTarget=nullptr;
				bHasCombatTarget=false;
			}
			return;
		}
	
	if(LockOnPotentialTargets.Num() == 2)
	{
		if(CombatTarget==LockOnPotentialTargets[0])
		{					
			NextLockedEnemy=Cast<AEnemy>(LockOnPotentialTargets[1]);
			if(NextLockedEnemy->Alive())
			{
				CombatTarget = NextLockedEnemy;
				LockedOnEnemy=CombatTarget;
			}
		}
		else if(CombatTarget==LockOnPotentialTargets[1])
		{
			NextLockedEnemy=Cast<AEnemy>(LockOnPotentialTargets[0]);
			if(NextLockedEnemy->Alive())
			{
				CombatTarget = NextLockedEnemy;
				LockedOnEnemy=CombatTarget;
			}
		}
		return;
	}
	
	if(LockOnPotentialTargets.Num() > 2)
	{
		UE_LOG(LogTemp,Warning,TEXT("%i"), LockOnPotentialTargets.Num());

		const int32 Size = LockOnPotentialTargets.Num();
		const int32 j = Size - 1;
		const int32 Rand = FMath::RandRange(0, j);
		if(CombatTarget!=GetClosestEnemy())
		{
			NextLockedEnemy=GetClosestEnemy();
			CombatTarget = NextLockedEnemy;
			LockedOnEnemy=CombatTarget;				
		}
		else if(CombatTarget==GetClosestEnemy())
		{
			NextLockedEnemy=Cast<AEnemy>(LockOnPotentialTargets[Rand]);
			CombatTarget = NextLockedEnemy;
			LockedOnEnemy=CombatTarget;
		}	
	}
		
}

void AMainChar::GetLockOnPotentialTargets(TArray<AActor*> OverlappingEnemies, TArray<AActor*>& CheckedEnemies) const
{

	CheckedEnemies.Reset(OverlappingEnemies.Num());

	for(AActor* Enemies : OverlappingEnemies)
	{
		const AEnemy* CheckingTarget = Cast<AEnemy>(Enemies);
		if(CheckingTarget)
		{
			if(CheckingTarget->AIController->LineOfSightTo(this))
			{
				CheckedEnemies.Add(Enemies);
			}
		}
	}

}

void AMainChar::SwitchLevels(FName LevelName) const
{
	const UWorld* World = GetWorld();
	if(World)
	{
		const FString CurrentLevelString = World->GetMapName();
		const FName CurrentLevelName(*CurrentLevelString);
		if(CurrentLevelName != LevelName)
		{
			UGameplayStatics::OpenLevel(World, LevelName);		

		}
	}
}

void AMainChar::SaveGame()
{
	UFirstSaveGame* SaveGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	SaveGameInstance->CharacterStats.Health= Health;
	SaveGameInstance->CharacterStats.MaxHealth = MaxHealth;
	SaveGameInstance->CharacterStats.Mana = Mana;
	SaveGameInstance->CharacterStats.MaxMana = MaxMana;
	SaveGameInstance->CharacterStats.Stamina = Stamina;
	SaveGameInstance->CharacterStats.MaxStamina = MaxStamina;
	SaveGameInstance->CharacterStats.Location = GetActorLocation();
	SaveGameInstance->CharacterStats.Rotation = GetActorRotation();
	SaveGameInstance->CharacterStats.Coins = Coins;

		
	if(EquippedWeapon)
	{
		SaveGameInstance->CharacterStats.EquippedMainHandName = EquippedWeapon->Name;
	}


	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->PlayerName, SaveGameInstance->UserIndex);



}

void AMainChar::LoadGame(bool SetPosition)
{
	const UFirstSaveGame* LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::CreateSaveGameObject(UFirstSaveGame::StaticClass()));

	LoadGameInstance = Cast<UFirstSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->PlayerName, LoadGameInstance->UserIndex));



	Health = LoadGameInstance->CharacterStats.Health;
	MaxHealth = LoadGameInstance->CharacterStats.MaxHealth;
	Stamina = LoadGameInstance->CharacterStats.Stamina;
	MaxStamina = LoadGameInstance->CharacterStats.MaxStamina;
	Mana = LoadGameInstance->CharacterStats.Mana;
	MaxMana = LoadGameInstance->CharacterStats.MaxMana;
	Coins = LoadGameInstance->CharacterStats.Coins;
	


	if(WeaponStorage)
	{
		AItemStorage* Weapons = GetWorld()->SpawnActor<AItemStorage>(WeaponStorage);
		if(Weapons)
		{
			if(Weapons->WeaponMap.Contains(LoadGameInstance->CharacterStats.EquippedMainHandName))
			{
				AWeapon* WeaponToEquip = GetWorld()->SpawnActor<AWeapon>(Weapons->WeaponMap[LoadGameInstance->CharacterStats.EquippedMainHandName]);
				if(WeaponToEquip)
				{
					WeaponToEquip->Equip(this);
				}
			}
		}
	}

	if(SetPosition)
	{
		SetActorLocation(LoadGameInstance->CharacterStats.Location);
		SetActorRotation(LoadGameInstance->CharacterStats.Rotation);
		
	}

	SetMovementStatus(EMovementStatus::EMS_Normal);
	GetMesh()->bPauseAnims=false;

}

// ReSharper disable once CppMemberFunctionMayBeConst Can't be const if bound
void AMainChar::TogglePause()
{
	if(MainPlayerController)
	{
		MainPlayerController->TogglePause();
	}
}