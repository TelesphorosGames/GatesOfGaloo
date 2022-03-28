// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GATESOFGALOO_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	// Holds the widget in Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<class UUserWidget> HudOverlayAsset;

	// Holds the widget once created
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	UUserWidget* HudOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UUserWidget> WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	UUserWidget* EnemyHealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UUserWidget> WPauseMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	UUserWidget* PauseMenu;


	bool bPauseMenuVisible;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisplayPauseMenu();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void RemovePauseMenu();

	void TogglePause();

	bool bEnemyHealthBarVisible;

	void DisplayEnemyHealthBar();

	void RemoveEnemyHealthBar();


	FVector EnemyLocation;

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	
};
