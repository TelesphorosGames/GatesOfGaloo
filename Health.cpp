// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

AHealth::AHealth()
{

}


void AHealth::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    UE_LOG(LogTemp, Warning, TEXT("Health override ONOVERLAPBEGIN Function Called"));
    Destroy();

}


void AHealth::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    UE_LOG(LogTemp, Warning, TEXT("Health override ONOVERLAPEND Function Called"));
    
}