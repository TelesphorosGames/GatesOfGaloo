// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "SpiderEnemy.generated.h"

/** A basic child of AEnemy set up for a spider version of an enemy.
 * 
 */
UCLASS()
class GATESOFGALOO_API ASpiderEnemy : public AEnemy
{
	GENERATED_BODY()

	ASpiderEnemy();

	
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	const USkeletalMeshSocket* SpiderParticleSocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class UBoxComponent* SpiderCombatCollision;
 
	virtual void BeginPlay() override;

	virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void ActivateCollision() override;
	
	virtual void DeActivateCollision() override;

	virtual void Attack() override;

	virtual void AttackEnd() override;


};
