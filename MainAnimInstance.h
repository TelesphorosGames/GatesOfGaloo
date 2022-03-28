// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GATESOFGALOO_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
 

public:

	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category= "Animations")
	void UpdateAnimationProperties();

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementAngle;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bJumpJustPressed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bMyCrouch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bLockedOnPerspectiveForAnims = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bMySprint = false;

	UPROPERTY(BlueprintReadOnly)
	float TurnAtValue;

	UPROPERTY(BlueprintReadOnly)
	bool bCamIsFreeForAnim;

	UPROPERTY(BlueprintReadOnly)
	float TurnAtValueTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* CharPawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	class AMainChar* MainChar;

	
	
};
