// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BadPickUp.generated.h"

/** This is a sibling of AItem that can be used as a base class for a bad pick up
 *  NO MESH AT THIS LEVEL 
 */
UCLASS()
class GATESOFGALOO_API ABadPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABadPickUp();

	// Will be used as our root and collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Collision ")
	class USphereComponent* CollisionSphere;

	// Base Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Particles")
	class UParticleSystemComponent* IdleParticlesComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Particles")
	class UParticleSystem* OverlapEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Sounds")
	class USoundCue* OverlapSound;

	// Should the picup rotate?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Items")
	bool bRotate;

	// How fast should it rotate? Requiores bRotate to be true
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Items")
	float RotationRate;

	// Should the object oscilate up and down?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Items")
	bool bUpnDown;
	
	float RunningTime = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
