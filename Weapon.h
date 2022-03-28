// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

enum class EWeaponState :uint8
{
	EWS_Floating UMETA(DisplayName="FloatingPickup"),
	EWS_Equipped UMETA(DisplayName="Equipped"),

	EWS_MAX UMETA(DisplayName="DefaultMax")
};

UENUM(BluePrintType)
enum class EWeaponType :uint8
{
	EWT_Sword UMETA(DisplayName="Sword"),
	EWT_Bow UMETA(DisplayName="Bow"),
	EWT_Dagger UMETA(DisplayName="Dagger"),

	EWT_MAX UMETA(DisplayName="DefaultMax")

}; 

/** 
 * 
 */
UCLASS()
class GATESOFGALOO_API AWeapon : public AItem
{
	GENERATED_BODY()
public:

	AWeapon();

	UPROPERTY(EditDefaultsOnly, Category="SaveData")
	FString Name;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class USkeletalMeshComponent* SkelMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Stuff")
	class UBoxComponent* DamageRange;


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Equip(class AMainChar* Char);

	AMainChar* OwningMainCharRef;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	class USoundCue* OnEquipSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	USoundCue* SwingSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	TSubclassOf<UDamageType> DamageTypeClass;
	
	EWeaponState WeaponState;

	FORCEINLINE void SetWeaponState(EWeaponState State) {WeaponState = State;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Stuff")
	float Damage;
	
	const USkeletalMeshSocket* ParticleSocket; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "My Stuff | AI")
	class UParticleSystem* HitParticles;

	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	
	UFUNCTION(BlueprintCallable)
	void DeActivateCollision();
	
protected:

	void BeginPlay() override;
};
