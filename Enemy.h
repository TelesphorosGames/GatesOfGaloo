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

UCLASS()
class GATESOFGALOO_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	bool bOverlappingChar = false;

	bool bViewToMain = false;
	
	bool bHasValidTarget = false;

	
	AMainChar* MainCharEnemyRef;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	EEnemyMovementStatus EnemyMovementStatus;

	FORCEINLINE EEnemyMovementStatus GetEnemyMovementStatus(){return EnemyMovementStatus;}
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus Status){EnemyMovementStatus = Status;}

	


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class AAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float Health; 
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	float UnarmedDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UDamageType> UnarmedDamageTypeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	class UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	USoundCue* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	USoundBase* AgroSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "My Stuff")
	class UAnimMontage* CombatMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	bool bAttacking;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float AttackMaxTime;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	virtual void NoMoreHealth(AActor* Causer);

	FTimerHandle DisappearTimer;

	UFUNCTION()
	void Disappear();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff")
	class UParticleSystem* DisappearParticles;


	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "My Stuff | Items")
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff");
	bool bOverlappingCombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff");
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

	bool Alive();



};
