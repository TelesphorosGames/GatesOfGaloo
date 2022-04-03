// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyMovementStatus :uint8
{
	EMS_Idle UMETA(DisplayName="Idle"),
	EMS_MoveToTarget UMETA(DisplayName="MoveToTarget"),
	EMS_Attacking UMETA(DisplayName="Attacking"),
	EMS_Dead UMETA(DisplayName="Dead"),

	EMS_MAX UMETA(DisplayName="DefaultMax")
};

/* The basic enemy class. Contatins functions for movement, AI navigation, simple character detection via
 * sphere component overlap, attack, death, and animation properties. Can be inherited from to create a custom
 * enemy class, such as ASkelEnemy or ASpiderEnemy. 
 */
UCLASS()
class GATESOFGALOO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Is the enemy overlapping the character's range sphere, or any aspect of the character?
	bool bOverlappingChar = false;

	// Does the enemy have a view to the main character?
	bool bViewToMain = false;

	// Does the enemy have a valid attack target ( or is the character dead, incapacitated, etc? ) 
	bool bHasValidTarget = false;

	// A reference to the main character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="My Stuff")
	class AMainChar* MainCharEnemyRef;

	// Is enemy Idle, Moving, Attacking, or Dead? 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	EEnemyMovementStatus EnemyMovementStatus;
	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus() const {return EnemyMovementStatus;}
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status){EnemyMovementStatus = Status;}

	// The range that the enemy will detect the character from, smaller than the main character's lock on range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class USphereComponent* AgroSphere;

	// The enemy's attack range
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	USphereComponent* CombatSphere;

	// The AI Controller blessed to us by Epic Games
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class AAIController* AIController;

	// Current Enemy Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float Health;

	// Enemy Max Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float MaxHealth;

	// How much damage the enemy does with no weapon spawned / equipped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float UnarmedDamage;

	// Damage Type class for base unarmed damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UDamageType> UnarmedDamageTypeClass;

	// Particle effect played when the enemy is hit by player's wepaon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	UParticleSystem* HitParticles;

	// Sound plays when hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class USoundCue* HitSound;

	// Enemy's swing sound if desired to be set in C++
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	USoundCue* SwingSound;

	// Sound that plays when the enemy detects the character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	USoundBase* AgroSound;

	// Enemy's combat montage
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Stuff")
	UAnimMontage* CombatMontage;

	// Enemy currently attacking?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	bool bAttacking;

	// Handles how often the enemy can attack
	FTimerHandle AttackTimer;

	// Minimum time in seconds that the enemy's attacks will be spaced out by
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float AttackMinTime;

	// Maximum time in seconds that the enemy's attacks will be spaced out by
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float AttackMaxTime;

	// Handles when the enemy takes damage of any kind
	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Starts the enemy's death effect
	virtual void NoMoreHealth(AActor* Causer);

	// How long it takes the enemy to disappear after completely dead and anims are finished playing, etc
	FTimerHandle DisappearTimer;
	// Makes enemy disappear and deletes them from memory
	UFUNCTION()
	void Disappear();

	// Particles for disappear effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	UParticleSystem* DisappearParticles;

	// A pointer to the Enemy's weapon, if equipped from a child class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff")
	class AWeapon* EnemyEquippedWeapon;
	virtual void SetEquippedEnemyWeapon(AWeapon* WepToSet);
	virtual AWeapon* GetEquippedEnemyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMainChar* Target);

	// Is the player overlapping our combat sphere? Used for attack checks
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	bool bOverlappingCombatSphere;

	// A pointer to the enemy's current combat target
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	AMainChar* CombatTarget;

	UFUNCTION(BlueprintCallable)
	virtual void ActivateCollision();
	
	UFUNCTION(BlueprintCallable)
	virtual void DeActivateCollision();

	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	bool Alive() const;
};
