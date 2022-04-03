// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

/* Volume to move us between levels in the game
 *
 */
UCLASS()
class GATESOFGALOO_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class UBoxComponent* TransitionVolume;

	class UBillboardComponent* Billboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "My Stuff")
	FName NextLevelName;

};
