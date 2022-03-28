// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "TimerManager.h"



// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	/** Here in the constructor of FloorSwitch we are defining our declared UPROPERTies
	 * 
	 */
	 
	
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = Box;

	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);


	

	FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	FloorSwitch->SetupAttachment(GetRootComponent());

	Door=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	Door->SetupAttachment(GetRootComponent());



}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	Box->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);

	InitialDoorLocation = Door->GetComponentLocation();
	InitialSwitchLocation = FloorSwitch->GetComponentLocation();
	
}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
UE_LOG(LogTemp, Warning, TEXT("Overlap Begin!"));
RaiseDoor();
LowerDoorSwitch();
bCharacterOnSwitch=true;
}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
UE_LOG(LogTemp, Warning, TEXT("Overlap End!"));

GetWorldTimerManager().SetTimer(SwitchTimerHandle, this, &AFloorSwitch::CloseDoor, Switchtime);
bCharacterOnSwitch = false;

}

void AFloorSwitch::UpdateDoorLocation(float z)
{
	FVector NewLocation = InitialDoorLocation;
	NewLocation.Z += z;
	Door->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateDoorSwitchLocation(float z)
{
	
	FVector NewLocation = InitialSwitchLocation;
	NewLocation.Z += z;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{

	if(!bCharacterOnSwitch)
	{
	LowerDoor();
	RaiseDoorSwitch();
	}
}