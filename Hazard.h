// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Hazard.generated.h"

/** A child of Item used to damage our character upon overlap 
 * 
 */
UCLASS()
class GATESOFGALOO_API AHazard : public AItem
{
	GENERATED_BODY()

	AHazard();

public:

	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Damage")
	float Damage = 10.f;
	
};
