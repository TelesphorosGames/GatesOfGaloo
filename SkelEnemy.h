// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SkelEnemy.generated.h"

/** A Skeleton version of an enemy that inherits from our AEnemy class. Has functionality to spawn a weapon.
 * 
 */

UCLASS()
class GATESOFGALOO_API ASkelEnemy : public AEnemy
{
	GENERATED_BODY()

	ASkelEnemy();
public:
	virtual void BeginPlay() override;

	UPROPERTY()
	const USkeletalMeshSocket* SwordSocket;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WepClass;

	virtual void DeathEnd() override;	

	UPROPERTY(VisibleAnywhere)
	AWeapon* Wep;

	virtual void SetEquippedEnemyWeapon(AWeapon* WepToSet) override;

	virtual AWeapon* GetEquippedEnemyWeapon() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	virtual void NoMoreHealth(AActor* Causer) override;
	
};
 