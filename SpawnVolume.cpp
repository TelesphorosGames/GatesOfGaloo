// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Critter.h"
#include "AIController.h"
#include "Enemy.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox=CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if(ActorOne && ActorTwo && ActorThree && ActorFour)
	{
		SpawnArray.Add(ActorOne);
		SpawnArray.Add(ActorTwo);
		SpawnArray.Add(ActorThree);
		SpawnArray.Add(ActorFour);
	}
	
} 

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
	
}


void ASpawnVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if(ToSpawn)
	{
		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;

		if (World)
	{
		
		AActor* NewSpawnedActor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f), SpawnParams);
		
		AEnemy* MaybeEnemy = Cast<AEnemy>(NewSpawnedActor);
		if(MaybeEnemy)
		{
			MaybeEnemy->SpawnDefaultController();
			AAIController* AICont = Cast<AAIController>(MaybeEnemy->GetController());
			if(AICont)
			{
				MaybeEnemy->AIController = AICont;
			}
		}
	}
	}
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if(SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, SpawnArray.Num()-1);
		return SpawnArray[Selection];
	}
	else
	{
		return nullptr;
	}
}
