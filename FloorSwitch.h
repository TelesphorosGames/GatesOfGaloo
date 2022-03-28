// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class GATESOFGALOO_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	/** Declaring a pointer variable called Box of type UBoxComponent
	 * This will serve as our trigger mechanism, aka trigger volume
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class UBoxComponent* Box;


	// The actual mesh property of this floor switch
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class UStaticMeshComponent* FloorSwitch;

	/** This is a pointer variable called Door of type UStaticMeshComponent
	 *   It will serve as the door that is opened by the trigger overlap
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class UStaticMeshComponent* Door;

	UPROPERTY(BlueprintReadWrite, Category = "My Stuff")
	FVector InitialDoorLocation;

	UPROPERTY(BlueprintReadWrite, Category = "My Stuff")
	FVector InitialSwitchLocation;

	// IF BINDING A FUNCTION TOO A DELEGATE IT MUST BE A UFUNCTION */
	UFUNCTION(Category = "My Stuff")
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(Category = "My Stuff")
	void OnOverlapEnd( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category= "My Stuff")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category= "My Stuff")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category= "My Stuff")
	void RaiseDoorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category= "My Stuff")
	void LowerDoorSwitch();

	UFUNCTION(BlueprintCallable, Category = "My Stuff")
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "My Stuff")
	void UpdateDoorSwitchLocation(float z);



	FTimerHandle SwitchTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float Switchtime = 2.f;

	
	bool bCharacterOnSwitch = false;

	void CloseDoor();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
