// Fill out your copyright notice in the Description page of Project Settings.


#include "SpiderEnemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"
#include "MainChar.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimInstance.h"
#include "AIController.h"


ASpiderEnemy::ASpiderEnemy()
{
    SpiderCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SpiderCombatCollision"));
    SpiderCombatCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale,FName("EnemySocket"));

    SpiderCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SpiderCombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SpiderCombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    SpiderCombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

    bAttacking=false;



}


void ASpiderEnemy::BeginPlay()
{
    Super::BeginPlay();

    SpiderCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEnemy::CombatOnOverlapBegin);
   

    SpiderParticleSocket = GetMesh()->GetSocketByName("HitSocket");

    
  
}

void ASpiderEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::CombatSphereOnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	// if(OtherActor && Alive())
	// {
	// 	AMainChar* Main = Cast<AMainChar>(OtherActor);
	// 	if(Main)
	// 	{
	// 		CombatTarget=Main;
	// 		Attack();
	// 		UE_LOG(LogTemp,Warning,TEXT("ATTACKING"));
	// 		bOverlappingCombatSphere = true;
	// 	} 
	// }
}

void ASpiderEnemy::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if(OtherActor && Alive())
    {
        AMainChar* Main = Cast<AMainChar>(OtherActor);
        if(Main)
        {
            if(SpiderParticleSocket)
            {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, SpiderParticleSocket->GetSocketLocation(GetMesh()), FRotator(0.f), false);
            }
            if(Main->HitSound)
            {
            UGameplayStatics::PlaySound2D(this, Main->HitSound);
            }
            if(UnarmedDamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Main, UnarmedDamage, GetInstigatorController(), this, UnarmedDamageTypeClass);
            }
            if(Main->Health <= 0.f)
            {
            AEnemy* LastHitter = Cast<AEnemy>(GetOwner());
            if(LastHitter)
            {
                LastHitter->bHasValidTarget = false;
            }
            }
        }
    }
}


void ASpiderEnemy::ActivateCollision()
{
    SpiderCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
}
	

void ASpiderEnemy::DeActivateCollision()
{
    SpiderCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASpiderEnemy::Attack()
{
    if(Alive())
    {

        if(AIController)
        {
            AIController->StopMovement();
            AEnemy::SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attacking);

        }
        if(!bAttacking)
        {
            bAttacking=true;
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if(AnimInstance)
            {
                AnimInstance->Montage_Play(CombatMontage, 1);
                AnimInstance->Montage_JumpToSection(FName("AttackOne"), CombatMontage);
            }
        }
    }
    
}
  
void ASpiderEnemy::AttackEnd()
{
    Super::AttackEnd();
    
        // bAttacking = false;
        // AEnemy::SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
        
        // if(bOverlappingCombatSphere)
        // {
        //     Attack();
        // }

    
}