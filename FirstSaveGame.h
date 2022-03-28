// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "FirstSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float Stamina;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float MaxStamina;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float Mana;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	float MaxMana;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	int32 Coins;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	FVector Location;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	FString EquippedMainHandName;

	UPROPERTY(VisibleAnywhere, Category = "My Stuff")
	FString LevelName;

};




/**
 * 
 */
UCLASS()
class GATESOFGALOO_API UFirstSaveGame : public USaveGame
{
	GENERATED_BODY()


public:

UFirstSaveGame();

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString PlayerName;
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
 
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FCharacterStats CharacterStats;


	
};
