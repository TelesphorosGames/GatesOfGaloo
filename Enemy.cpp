// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainChar.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/CapsuleComponent.h"
#include "Weapon.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "MainPlayerController.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	AgroSphere->SetupAttachment(GetRootComponent());

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	CombatSphere->SetupAttachment(GetRootComponent());

	AgroSphere->InitSphereRadius(1200.f);
	CombatSphere->InitSphereRadius(100.f);

	bAttacking=false;
 
	bOverlappingCombatSphere = false;

	Health = 40.f;
	MaxHealth = 40.f;

	AttackMinTime = 0.5f;
	AttackMaxTime = 3.5f;

	EnemyMovementStatus = EEnemyMovementStatus::EMS_Idle;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());

	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);


	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);



	MainCharEnemyRef = Cast<AMainChar>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(MainCharEnemyRef)
	{
		UE_LOG(LogTemp,Warning,TEXT("TRUESKIDOSKI"));
	}

	


}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (bOverlappingChar && EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking && Alive())
	{
		
		// if(EnemyMovementStatus == EEnemyMovementStatus::EMS_MoveToTarget)
		// {
			TArray<AActor*> OverlappedActors;
			GetOverlappingActors(OverlappedActors);
			for (AActor* Actors : OverlappedActors)
			{
				// FString ActorName = Actors->GetName();
			
				AMainChar* Main = Cast<AMainChar>(Actors);
				if(Main)
				{
					if(Main->bIsValidTarget == false)
					{
						GetWorldTimerManager().ClearTimer(AttackTimer);
						return;
					}
					if(AIController)
					{
						if(AIController->LineOfSightTo(Main))
						{
							MoveToTarget(Main);
						
							
							// AIController->SetFocus(Main);

						}

							// UE_LOG(LogTemp,Warning,TEXT("ACTORS: %s"), *ActorName);
					}
				}
			}
		// }
		
	}
	if(bOverlappingCombatSphere && Alive())
	{
		AIController->StopMovement();
		
		if(MainCharEnemyRef->CombatTarget==nullptr  ||MainCharEnemyRef->CombatTarget && MainCharEnemyRef->CombatTarget->Health <= 0.f)
		{
			MainCharEnemyRef->SetCombatTarget(this);
			
		}
		
	}

	// TOTALLY USELESS CODE: 
	
	// if(AIController && AIController->LineOfSightTo(MainCharEnemyRef) == false)
	// {
	// 	if(MainCharEnemyRef && MainCharEnemyRef->CombatTarget==this)
	// 	{
	// 		MainCharEnemyRef->SetHasCombatTarget(false);
	// 		MainCharEnemyRef->SetCombatTarget(nullptr);
	// 		MainCharEnemyRef->MainPlayerController->RemoveEnemyHealthBar();
	// 	}
	// }




}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if(OtherActor && Alive())
	{

		AMainChar* Main = Cast<AMainChar>(OtherActor);
		if(Main)
		{
			if(AIController)
			{
				bOverlappingChar = true;
				if(AIController->LineOfSightTo(Main) )
				{
					MoveToTarget(Main);
					if(AgroSound)
					{
						UGameplayStatics::PlaySoundAtLocation(GetWorld(), AgroSound, GetOwner()->GetActorLocation());
					}
					if(Main->CombatTarget==nullptr)
					{
						Main->SetCombatTarget(this);
						Main->LockedOnEnemy = this;
						Main->bHasCombatTarget = true;
						
					}
				}

			}	
		}
	}
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor && Alive())
	{
		AMainChar* Main = Cast<AMainChar>(OtherActor);
		if(Main)
		{
			if(Main->LockedOnEnemy == this)
			{
				Main->CycleTarget();
				Main->SetHasCombatTarget(false);
				Main->SetCombatTarget(nullptr);
				Main->LockedOnEnemy = nullptr;
				Main->bCurrentlyLocked = false;
				
			}
			
				
			bOverlappingChar = false;
			bHasValidTarget = false;
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);

			if(AIController)
			{
				AIController->StopMovement();
				AIController->ClearFocus(EAIFocusPriority::Gameplay);
			}
			
			if(Main->MainPlayerController)
			{
				Main->MainPlayerController->RemoveEnemyHealthBar();
			}
		}
	}
}

void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
		if(OtherActor && Alive())
		{
			AMainChar* Main = Cast<AMainChar>(OtherActor);
			if(Main)
			{

						if(Main->MainPlayerController)
							{
								Main->MainPlayerController->DisplayEnemyHealthBar();
								if(Main->CombatTarget==nullptr || Main->CombatTarget->Health <= 0.f)
								{
								Main->SetCombatTarget(this);
								Main->SetHasCombatTarget(true);
								AIController->SetFocus(Main);
						

								}
								
													
							CombatTarget=Main;

							}
			
				bHasValidTarget = true;
				SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
				if(Main->bIsValidTarget)
				{
				Attack();
				bOverlappingCombatSphere = true;

				}
			}
		}

	
}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	if(OtherActor && Alive())
	{
		AMainChar* Main = Cast<AMainChar>(OtherActor);
		if(Main)
		{
					


			bOverlappingCombatSphere = false;

			if(EnemyMovementStatus != EEnemyMovementStatus::EMS_Attacking && Alive())
			{
				MoveToTarget(Main);				
					AIController->ClearFocus(EAIFocusPriority::Gameplay);

				}
			GetWorldTimerManager().ClearTimer(AttackTimer);

		}
	}
}

void AEnemy::MoveToTarget(class AMainChar* Target)
{
	if(Alive())
	{

		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
		if(AIController)
		{
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalActor(Target);
			MoveRequest.SetAcceptanceRadius(50.f);

			FNavPathSharedPtr NavPath;

			AIController->MoveTo(MoveRequest, &NavPath);
		}


		// TArray<FNavPathPoint> PathPoints = NavPath->GetPathPoints();

		// for (FNavPathPoint Point : PathPoints)
		// {
		// 	FVector Location = Point.Location;
			

		// 	UKismetSystemLibrary::DrawDebugSphere(this, Location, 30.f, 8, FLinearColor::Red, 5.f, 15.f);
			
		// }
	}


}

void AEnemy::ActivateCollision()
{

}

void AEnemy::DeActivateCollision()
{

}

void AEnemy::Attack()
{
	if(Alive() && bHasValidTarget)
	{

		UE_LOG(LogTemp,Warning,TEXT("ATTACKING"));
		if(AIController)
		{
			AIController->StopMovement();
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);
		}
		if(!bAttacking)
		{
			bAttacking=true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if(AnimInstance)
			{
				int Section = FMath::RandRange(0,3);
					switch (Section)
					{
						case 0:
				AnimInstance->Montage_Play(CombatMontage, .8f);
				AnimInstance->Montage_JumpToSection("AttackOne");
					break;
						case 1:
				AnimInstance->Montage_Play(CombatMontage, .8f);
				AnimInstance->Montage_JumpToSection("AttackTwo");
					break;
						case 2:
				AnimInstance->Montage_Play(CombatMontage, .8f);
				AnimInstance->Montage_JumpToSection("AttackThree");
					break;
						case 3:
				AnimInstance->Montage_Play(CombatMontage, .8f);
				AnimInstance->Montage_JumpToSection("AttackFour");
					break;
				default:
						;
					}
			}
		}
	}
}

void AEnemy::AttackEnd()
{

	
	   bAttacking = false;
    SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
    
    if(bOverlappingCombatSphere)
    {
		float AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
        // Attack();
    

	}
}

void AEnemy::SetEquippedEnemyWeapon(AWeapon* WepToSet)
{

}

AWeapon* AEnemy::GetEquippedEnemyWeapon()
{
return EnemyEquippedWeapon;
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
		Health -= DamageAmount;
		NoMoreHealth(DamageCauser->GetOwner());
		UE_LOG(LogTemp, Error, TEXT("%s"), *DamageCauser->GetOwner()->GetName());
		// AMainChar* Main = Cast<AMainChar>(DamageCauser);
		// if(Main)
		// {
		// 	Main->MainPlayerController->RemoveEnemyHealthBar();
		// }

	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemy::NoMoreHealth(AActor* Causer)
{
	bOverlappingChar = false;
		CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->Deactivate();
	AMainChar* MainRef = Cast<AMainChar>(Causer);
	if(MainRef && MainRef->LockedOnEnemy == this && !(MainRef->LockedOnEnemy->Alive()))
	{
		UE_LOG(LogTemp,Warning,TEXT("REACHED"));
		AEnemy* NextTarget = MainRef->GetClosestEnemy();
		if(NextTarget && NextTarget->Alive())
		{
		MainRef->SetCombatTarget(NextTarget);
		MainRef->LockedOnEnemy = NextTarget;
			UE_LOG(LogTemp,Warning,TEXT("DOUBLE REACHED"));
		}
		else if(NextTarget && !(NextTarget->Alive()))
		{
			MainRef->CycleTarget();
				UE_LOG(LogTemp,Warning,TEXT("TRIPLE REACHED"));
		}
		else
		{
			MainRef->SetCombatTarget(nullptr);
			MainRef->SetHasCombatTarget(false);
			MainRef->LockedOnEnemy = nullptr;
			MainRef->bCurrentlyLocked = false;
		
		}
	}

	bHasValidTarget= false;
		AIController->ClearFocus(EAIFocusPriority::Gameplay);

	
		
    	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if(AnimInstance && CombatMontage)
		{
			AnimInstance->Montage_Play(CombatMontage);
			AnimInstance->Montage_JumpToSection(FName("Death"), CombatMontage);
		}
	GetWorldTimerManager().SetTimer(DisappearTimer, this, &AEnemy::Disappear, 5.0f);
	
}

void AEnemy::DeathEnd()
{

	GetMesh()-> bPauseAnims = true;

	// GetMesh()->bNoSkeletonUpdate = true;
	UE_LOG(LogTemp,Warning, TEXT("DeathEnd Reached"));
	UE_LOG(LogTemp,Warning,TEXT("%i"), Alive());
	

	
	   
}

bool AEnemy::Alive()
{
return GetEnemyMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
	if(DisappearParticles)
	{
		const USkeletalMeshSocket* DisappearParticleSocket = GetMesh()->GetSocketByName("headSocket");
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DisappearParticles, DisappearParticleSocket->GetSocketLocation(GetMesh()), FRotator(0.f), true);
	}
	Destroy();
	
	
}

