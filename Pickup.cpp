// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "MainChar.h"


APickup::APickup()
{
    MyRandomHeight = FMath::RandRange(20,50);

    HealthIncreaseAmount = 10.f;
}


void APickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    UE_LOG(LogTemp, Warning, TEXT("Pickup override ONOVERLAPBEGIN Function Called"));
    if(OtherActor)
    {
        AMainChar* Main = Cast<AMainChar>(OtherActor);

        
        if(Main)
        {
            Main->IncreaseHealth(HealthIncreaseAmount);
            Destroy();
        }
    }


}


void APickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    
} 