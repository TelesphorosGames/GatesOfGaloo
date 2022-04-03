// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Coin_Item.generated.h"
 
/** The basic coin class, child of AItem
 * 
 */
UCLASS()
class GATESOFGALOO_API ACoin_Item : public AItem
{
	GENERATED_BODY()

	ACoin_Item();

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff | Coins ")
	int32 CoinCount = 0;

	virtual void Tick(float DeltaTime) override;
};
