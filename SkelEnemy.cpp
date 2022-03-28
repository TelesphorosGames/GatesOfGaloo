// Fill out your copyright notice in the Description page of Project Settings.


#include "SkelEnemy.h"
#include "Weapon.h"
#include "Engine/SkeletalMesh.h"
#include "Components/CapsuleComponent.h"



ASkelEnemy::ASkelEnemy()
{
    
    
  
}

void ASkelEnemy::BeginPlay()
{
    Super::BeginPlay();
  
    Wep = GetWorld()->SpawnActor<AWeapon>(WepClass);
	
	Wep->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, (TEXT("SwordSocket")));
    Wep->SetOwner(this);

    EnemyEquippedWeapon = Wep;
    
    
}

void ASkelEnemy::SetEquippedEnemyWeapon(AWeapon* WepToSet)
{



}

AWeapon* ASkelEnemy::GetEquippedEnemyWeapon()
{
	return EnemyEquippedWeapon;
}

float ASkelEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health -= DamageAmount;
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
		NoMoreHealth(DamageCauser->GetOwner());
	}
	else
	{
		Health -= DamageAmount;
	} 
    return DamageAmount;
}

void ASkelEnemy::NoMoreHealth(AActor* Causer)
{
	Super::NoMoreHealth(Causer);

	
    // UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 	if(AnimInstance && CombatMontage)
	// 	{
	// 		AnimInstance->Montage_Play(CombatMontage);
	// 		AnimInstance->Montage_JumpToSection(FName("Death"));
	// 	}

	// GetWorldTimerManager().SetTimer(DisappearTimer, this, &AEnemy::Disappear, 12.5f);

        
		
	
    
}

void ASkelEnemy::DeathEnd()
{

	Super::DeathEnd();

	
	// GetMesh()->bNoSkeletonUpdate = true;
	
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    GetMesh()->SetSimulatePhysics(true);
    GetEquippedEnemyWeapon()->SetActorEnableCollision(false);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	Wep->Destroy();
	
}