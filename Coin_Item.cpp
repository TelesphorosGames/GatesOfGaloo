// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin_Item.h"
#include "MainChar.h"

ACoin_Item::ACoin_Item()
{
    CoinCount=1.f;
    if(bRandomHeight)
    {
    MyRandomHeight = FMath::RandRange(20,50);
    }
}

void ACoin_Item::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACoin_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
    UE_LOG(LogTemp, Warning, TEXT("ACoin_Item override ONOVERLAPBEGIN Function Called"));

    
    if(OtherActor)
    {
    AMainChar* Main = Cast<AMainChar>(OtherActor);
    if(Main)
    {
        Main->IncreaseCoins(CoinCount);
        Destroy();
    }
    }
    
}


void ACoin_Item::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
    UE_LOG(LogTemp, Warning, TEXT("ACoin_Item override ONOVERLAPEND Function Called"));
    
}

