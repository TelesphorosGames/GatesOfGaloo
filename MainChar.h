// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainChar.generated.h"


	

	//**
		//Declaring Enums for Movement Status and Stamina Status :
		//These will affect character abilities and stamina drain rate

UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal UMETA(DisplayName="Normal"),
	EMS_Running UMETA(DisplayName="Running"),
	EMS_Crouching UMETA(DisplayName="Crouching"),
	EMS_Sprinting UMETA(DisplayName="Sprinting"),
	EMS_Dead UMETA(DislayName="Dead"),

	EMS_MAX UMETA(DisplayName="DefaultMax")

};

UENUM(BluePrintType)
enum class EStaminaStatus : uint8
{ 
	ESS_Normal UMETA(DisplyName = "Normal"),
	ESS_BelowMinimum UMETA(DisplayName = "BelowMin"),
	ESS_Drained UMETA(DisplayName = "Drained"),
	ESS_Recovering UMETA(DisplayName = "Recovering"),
	
	ESS_MAX UMETA(DisplayName = "DefaultMAX")
};

	//**
	//**

UCLASS()
class GATESOFGALOO_API AMainChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainChar();

	UPROPERTY(EditDefaultsOnly, Category = "SaveData")
	TSubclassOf<class AItemStorage> WeaponStorage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff | Items")
	class AWeapon* EquippedWeapon;

	void SetEquippedWeapon(AWeapon* WepToSet);

	AWeapon* GetEquippedWeapon() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff | Items")
	class AWeapon* EquippedOffHand;

	void SetEquippedOffHand(AWeapon* WepToSet);

	AWeapon* GetEquippedOffHand() const;

	bool bMainHandEquipped;
	bool bOffHandEquipped;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff | Items")
	class AItem* ActiveOverlappingItem;

	FORCEINLINE void SetAOI(AItem* Item){ActiveOverlappingItem = Item;}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Enums")
	EMovementStatus MovementStatus; 

	UFUNCTION(BlueprintCallable)
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Enums")
	EStaminaStatus StaminaStatus;

	FORCEINLINE void SetStaminaStatus(EStaminaStatus Status){StaminaStatus = Status;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float StaminaDrainRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float MinSprintStamina = 25.f;

	float InterpToEnemySpeed;
	bool bInterpToEnemy;
	void SetInterpToEnemy(bool Interp);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class AEnemy* CombatTarget;

	bool bIsValidTarget = true;

	FORCEINLINE void SetCombatTarget(AEnemy* Target) { CombatTarget = Target ; } 

	FRotator GetLookAtRotationYaw(FVector Target) const;

	//  Declares a Spring Arm Component called CameraBoom and gives it its unreal engine property values, allowing us to access it from Blueprints
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Declares a Camera Component called Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* LockedOnCameraBoom;

	// Declares a Camera Component called Follow Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* LockedOnCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class USoundCue* HitSound;
	
	// Base Turn Rates to scale turning input by for camera functions 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	float BaseLookUpRate;

	void AdjustCameraLengthUp();
	void AdjustCameraLengthDown();

	UFUNCTION(BlueprintCallable)
	void AdjustPerspective();

	UFUNCTION(BlueprintCallable)
	void LockOnPerspective();

	UFUNCTION(BlueprintCallable)
	void CycleTarget();


	UPROPERTY()
	AEnemy* LockedOnEnemy = nullptr;

	UPROPERTY()
	AEnemy* NextLockedEnemy = nullptr;

	UPROPERTY()
	TArray<AActor*> AllOverlappingEnemies;

	UPROPERTY()
	TArray<AActor*> LockOnPotentialTargets;

	AEnemy* GetClosestEnemy();

	void GetLockOnPotentialTargets(TArray<AActor*> OverlappingEnemies, TArray<AActor*>& CheckedEnemies) const;
	
	bool bCurrentlyLocked = false;
	
	bool bEnemyInRange = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="My Stuff")
	bool bLockedOnPerspective = false;

	FRotator CurrentLookAtYawTarget = {0.f, 0.f, 0.f};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Camera")
	bool bFPPerspective = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bMyCrouchMain = false;

	// Functions called for input, forward/ backwards and side to side
	void MoveForward(float Value);
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void MyCrouch();

	void MyRun();

	UFUNCTION(BlueprintCallable)
	void MySprint();
	

	
	UFUNCTION(BlueprintCallable, Category= "My Stuff")
	void EquipBow();

	UFUNCTION(BlueprintCallable, Category= "My Stuff")
	void DisarmBow();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	bool bIsBowEquipped = false;

	UFUNCTION(BlueprintCallable, Category= "My Stuff")
	void EquipSword();

	UFUNCTION(BlueprintCallable, Category= "My Stuff")
	void DisarmSword();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	bool bIsSwordEquipped = false;

	UFUNCTION(BlueprintCallable)
	void AttachSwordToBack();

	UFUNCTION(BlueprintCallable)
	void DetachSwordFromBack();

	UFUNCTION(BlueprintCallable)
	void AttachDaggerToWaist();

	UFUNCTION(BlueprintCallable)
	void DetachDaggerFromWaist();

	UPROPERTY(BlueprintReadOnly)
	const USkeletalMeshSocket* SwordSocket;
	UPROPERTY(BlueprintReadOnly)
	
    const USkeletalMeshSocket* DaggerSocket;

	UPROPERTY(BlueprintReadOnly)
	const USkeletalMeshSocket* LeftHandSocket;
	UPROPERTY(BlueprintReadOnly)
    const USkeletalMeshSocket* RightHandSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bMidAnim = false;

	UPROPERTY(BlueprintReadOnly)
	bool bRunning = true;

	UPROPERTY(BlueprintReadOnly)
	bool bSprinting = false;

	// Functions called via input to turn and look up / down at a given rate @param Rate This is a normalized rate, i.e. 

	UFUNCTION(BlueprintCallable)
	void TurnAtRate(float Rate);


	void LookUpAtRate(float Rate);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	bool bAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	bool bJustJumped = false;

	virtual void Jump() override;

	virtual void StopJumping() override;
	
	UFUNCTION()
	void Attack();

	UFUNCTION(BlueprintCallable)
	void StopAttack();

	UFUNCTION(BlueprintCallable)
	void PlaySwingSound();

	UFUNCTION()
	void AttackOne();

	UFUNCTION()
	void AttackTwo();

	UFUNCTION()
	void AttackThree();

	UFUNCTION()
	void AttackFour();

	UFUNCTION()
	void AttackFive();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Stuff")
	class UAnimMontage* SwordCombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Stuff")
	class UAnimMontage* DaggerCombatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Stuff")
	class UAnimMontage* DualWieldMontage;

	bool bLMBDown;
	void LMBDown();
	void LMBUp();

	void TogglePause();



	FORCEINLINE class USpringArmComponent* GetCameraBoom() const {return CameraBoom;}
	FORCEINLINE class UCameraComponent* GetFollowCamera() const {return FollowCamera;}
	FORCEINLINE class USpringArmComponent* GetLockedOnCameraBoom() const {return LockedOnCameraBoom;}
	FORCEINLINE class UCameraComponent* GetLockedOnCamera() const {return LockedOnCamera;}
	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const {return FirstPersonCamera;}

	UPROPERTY(EditDefaultsOnly, Category = "My Stuff")
	float RunSpeedVar = 450.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "My Stuff")
	float SprintSpeedVar = 800.f;

	// STATS //

	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float Health = 0.f;

	UFUNCTION(BlueprintPure)
	float GetHealthPercentage() const;

	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float MaxMana = 100.f;
	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float Mana = 0.f;
 
	UFUNCTION(BlueprintPure)
	float GetManaPercentage() const;

	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float MaxStamina = 100.f;
	UPROPERTY(EditAnywhere, Category = "My Stuff | Stats")
	float Stamina = 0.f ;

	UFUNCTION(BlueprintPure)
	float GetStaminaPercentage() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Stuff | Stats")
	float AttackSpeed = 1.f;

	// PICKUPS //

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Pickups ")
	int32 Coins = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Pickups ")
	int32 Gemstones = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Pickups ")
	int32 GoldPieces = 0;

	FORCEINLINE void SetHasCombatTarget(bool HasTarget) { bHasCombatTarget = HasTarget;}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	bool bHasCombatTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	FVector CombatTargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class AMainPlayerController* MainPlayerController;

	UPROPERTY(EditDefaultsOnly, Category="My Stuff")
	TSubclassOf<AEnemy> EnemyFilter;

	UFUNCTION()
	void IncreaseHealth(float Amount);

	UFUNCTION() 
	void DecreaseHealth(float Amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	void NoMoreHealth();

	UFUNCTION(BlueprintCallable)
	void DeathEnd();

	UFUNCTION()
	void IncreaseCoins(int32 Coin); 
 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SwitchLevels(FName LevelName) const;

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame(bool SetPosition);



	

};
