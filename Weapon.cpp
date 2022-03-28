// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MainChar.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "MainPlayerController.h"

AWeapon::AWeapon()
{
    SkelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    SkelMesh->SetupAttachment(GetRootComponent());

    DamageRange = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageRange"));
    DamageRange->SetupAttachment(GetRootComponent());

    DamageRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageRange->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
    DamageRange->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    DamageRange->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);




    Damage = 5.f;

    WeaponState = EWeaponState::EWS_Floating;

}

void AWeapon::BeginPlay() 
{
    Super::BeginPlay();

    DamageRange->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
    DamageRange->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);

    ParticleSocket = SkelMesh->GetSocketByName("ParticleSocket");
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    
    if((WeaponState == EWeaponState::EWS_Floating) && OtherActor)
    {
        AMainChar* Main = Cast<AMainChar>(OtherActor);
        if(Main)
        {
            Main->SetAOI(this);
        }
    }
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if((WeaponState == EWeaponState::EWS_Floating) && OtherActor)
    {
        AMainChar* Main = Cast<AMainChar>(OtherActor);
        if(Main)
        {
            Main->SetAOI(nullptr);
        }
    }

}

void AWeapon::Equip(AMainChar* Char)
{
    if(Char)
    OwningMainCharRef = Char;
    
    {
        SkelMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
        SkelMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
        SkelMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
        CollisionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
        

        SkelMesh->SetSimulatePhysics(false);

        const USkeletalMeshSocket* LeftHandSocket = Char->GetMesh()->GetSocketByName("LeftHandSocket");
        const USkeletalMeshSocket* RightHandSocket = Char->GetMesh()->GetSocketByName("RightHandSocket");


        if (LeftHandSocket && WeaponType != EWeaponType::EWT_Sword && !(Char->EquippedOffHand))
        {
            
            LeftHandSocket->AttachActor(this, Char->GetMesh());
            bRotate = false;
            Char->SetEquippedOffHand(this);
            Char->GetEquippedOffHand();
            Char->SetAOI(nullptr);
            bUpnDown = false;
            Char->bIsBowEquipped = true;
             SetOwner(Char);

            WeaponState = EWeaponState::EWS_Equipped;

             if(OnEquipSound)
        {
            UGameplayStatics::PlaySound2D(this, OnEquipSound);
        }
        }
        if (RightHandSocket && WeaponType == EWeaponType::EWT_Sword && !(Char->EquippedWeapon))
        {
            RightHandSocket->AttachActor(this, Char->GetMesh());
            bRotate = false;
            Char->SetEquippedWeapon(this);
            Char->GetEquippedWeapon();
            Char->SetAOI(nullptr);
            bUpnDown = false;
            Char->bIsSwordEquipped = true;
            SetOwner(Char);

            WeaponState = EWeaponState::EWS_Equipped;

             if(OnEquipSound)
        {
            UGameplayStatics::PlaySound2D(this, OnEquipSound);
        }
        }

       
    }
}


void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    UE_LOG(LogTemp,Warning, TEXT("OVERLAPPED COMPONENT: %s, OTHER ACTOR : %s"), *OtherComp->GetName(), *OtherActor->GetName());
    if(OtherActor)
    {
       AEnemy* Enemy = Cast<AEnemy>(OtherActor);
       AMainChar* Main = Cast<AMainChar>(OtherActor);
       if(Enemy)
       {
           if(Enemy->HitParticles)
           {
               
               if(ParticleSocket)
               {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticles, ParticleSocket->GetSocketLocation(SkelMesh), FRotator(0.f), false);
               }
               
            }
           	if(Enemy->HitSound)
			{
				UGameplayStatics::PlaySound2D(this, Enemy->HitSound, 1);
			}
            if(DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Enemy, Damage, GetInstigatorController(), this, DamageTypeClass);
            }
            if(Enemy->Health <= 0.f)
            {
                if(OwningMainCharRef)
                {
                    OwningMainCharRef->MainPlayerController->RemoveEnemyHealthBar();
                    if(Enemy==OwningMainCharRef->CombatTarget)
                    {
                        if(OwningMainCharRef->bEnemyInRange==false)
                        {
                            OwningMainCharRef->CombatTarget = nullptr;
                            OwningMainCharRef->bHasCombatTarget = false;
                        }
                        else if(OwningMainCharRef->bEnemyInRange)
                        {
                           OwningMainCharRef->CombatTarget = OwningMainCharRef->GetClosestEnemy();
                
                        }
                    }
                }
                }
       }
       if(Main)
       {
            if(Main->HitParticles)
            {
                UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Main->HitParticles, ParticleSocket->GetSocketLocation(SkelMesh), FRotator(0.f), false);
            }
            if(Main->HitSound)
            {
                UGameplayStatics::PlaySound2D(this, Main->HitSound, 1);
            }
            if(DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(Main, Damage, GetInstigatorController(), this, DamageTypeClass);
                UE_LOG(LogTemp, Warning, TEXT("Damage Applied!"));
            }
            if(Main->Health <= 0.f)
            {
                Main->bIsValidTarget = false;
                
            AEnemy* LastHitter = Cast<AEnemy>(GetOwner());
            if(LastHitter)
            {
                LastHitter->bHasValidTarget = false;
            }
            }

       }
    }
}

void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


void AWeapon::ActivateCollision()
{
    DamageRange->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
	

void AWeapon::DeActivateCollision()
{
    DamageRange->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}