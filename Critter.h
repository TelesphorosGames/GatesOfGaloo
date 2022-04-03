// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"


/* A very basic class that can be possessed by the player input component, but has limited functions.
 * Simply a Pawn with camera and movement functionality 
 */

UCLASS()
class GATESOFGALOO_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

private:

	void MoveForward(float value);
	void MoveRight(float value);

	FVector CurrentVelocity = {0,0,0};

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 100.f;

};
