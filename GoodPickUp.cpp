// Fill out your copyright notice in the Description page of Project Settings.

#include "GoodPickUp.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"



// Sets default values
AGoodPickUp::AGoodPickUp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());

	IdleParticlesComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	IdleParticlesComp->SetupAttachment(GetRootComponent());

	bRotate = false;
	RotationRate = 45.f;
	bUpnDown= false;
}

// Called when the game starts or when spawned
void AGoodPickUp::BeginPlay()
{
	Super::BeginPlay();

	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AGoodPickUp::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AGoodPickUp::OnOverlapEnd);
	
}

// Called every frame
void AGoodPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime = GetGameTimeSinceCreation();

	if(bRotate)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Yaw += DeltaTime * RotationRate;
		SetActorRotation(Rotation);
	}
	if(bUpnDown)
	{
		FVector ItemLocation = GetActorLocation();
		float DeltaHeight = FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime);
		ItemLocation.Z += DeltaHeight * 30;
		SetActorLocation(ItemLocation);
		
		
	}



}

void AGoodPickUp::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("This is my Virtual void Good Pick Up On Overlap Begin function being called."));

	if(OverlapEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapEffect, GetActorLocation());
	}


	if(OverlapSound)
	{
		UGameplayStatics::PlaySound2D(this, OverlapSound);
	}
	
		Destroy();
}

void AGoodPickUp::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

		UE_LOG(LogTemp, Warning, TEXT("This is my Virtual void Good Pick Up OnOverlapEnd function being called."));
	
}