// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class GATESOFGALOO_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Stuff")
	class UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Stuff")
	TSubclassOf<AActor> ActorOne;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Stuff")
	TSubclassOf<AActor> ActorTwo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Stuff")
	TSubclassOf<AActor> ActorThree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Stuff")
	TSubclassOf<AActor> ActorFour;

	TArray<TSubclassOf<AActor>> SpawnArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	FVector GetSpawnPoint();

	UFUNCTION(BlueprintPure)
	TSubclassOf<AActor> GetSpawnActor();
 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "My Stuff")
	void SpawnOurPawn(UClass* ToSpawn, const FVector& Location);

};
