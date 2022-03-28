// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon.h"
#include "CoreMinimal.h"
#include "Enemy.h"
#include "SkelEnemy.generated.h"


/**
 * 
 */
UCLASS()
class GATESOFGALOO_API ASkelEnemy : public AEnemy
{
	GENERATED_BODY()

	ASkelEnemy();

	

public:



	void BeginPlay() override;

	const class USkeletalMeshSocket* SwordSocket;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WepClass;

	void DeathEnd() override;	

	class AWeapon* Wep;

	void SetEquippedEnemyWeapon(AWeapon* WepToSet) override;

	AWeapon* GetEquippedEnemyWeapon() override;

	float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	virtual void NoMoreHealth(AActor* Causer) override;
	
};
 