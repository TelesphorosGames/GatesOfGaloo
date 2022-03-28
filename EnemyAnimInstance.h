// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GATESOFGALOO_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;


	UFUNCTION(BlueprintCallable, Category= "Animations")
	void UpdateAnimationProperties();

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float SpiderMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementAngle;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AEnemy* Enemy;
	
};
