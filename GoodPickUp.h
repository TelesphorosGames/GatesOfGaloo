// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoodPickUp.generated.h"

/** This is a sibling of AItem that can be used as a base class for a good pick up
 *  For full comments, see class AItem
 */
UCLASS()
class GATESOFGALOO_API AGoodPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGoodPickUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff | Collision ")
	class USphereComponent* CollisionSphere;

	// Base Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Particles")
	UParticleSystemComponent* IdleParticlesComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Particles")
	UParticleSystem* OverlapEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Sounds")
	class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Items")
	bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Items")
	float RotationRate;

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
