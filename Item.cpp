// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "MainChar.h"

// Sets default values
AItem::AItem()
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
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	CollisionSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);
}

// Called every frame
void AItem::Tick(float DeltaTime)
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
		ItemLocation.Z += DeltaHeight * MyRandomHeight;
		SetActorLocation(ItemLocation, true);	
	}
}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	if(OtherActor)
    {
    AMainChar* Main = Cast<AMainChar>(OtherActor);
    if(Main)
    {
		if(OverlapEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapEffect, GetActorLocation());
		}
			
		if(OverlapSound)
		{
			UGameplayStatics::PlaySound2D(this, OverlapSound);
		}
			
		}
		// UE_LOG(LogTemp, Warning, TEXT("This is my Virtual void OnOverlapBegin function being called."));
		// if (OverlappedComponent && OtherActor)
		// {

		// 	// UE_LOG(LogTemp, Warning, TEXT("OL Comp : %s --- Other Actor : %s --- bFromSweep : %i --- other comp : %s" ), *OverlappedComponent->GetName(), *OtherActor->GetName(), bFromSweep, *OtherComp->GetName());
		
		// }
    }
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
		// UE_LOG(LogTemp, Warning, TEXT("This is my Virtual void OnOverlapEnd function being called."));
		// UPrimitiveComponent* Overlapped = OverlappedComponent;
		// FName OLName = Overlapped->GetFName();
		// UE_LOG(LogTemp, Warning, TEXT(" Overlapped Comp : %s"), *OLName.ToString());
}