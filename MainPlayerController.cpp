// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "MainChar.h"
#include "Blueprint/UserWidget.h"



void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if(HudOverlayAsset)
    {
        HudOverlay = CreateWidget<UUserWidget>(this, HudOverlayAsset);
        HudOverlay->AddToViewport();
        HudOverlay->SetVisibility(ESlateVisibility::Visible);


        
    }
    if(WEnemyHealthBar)
    {
        EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
        if(EnemyHealthBar)
        {
            EnemyHealthBar->AddToViewport();
            EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
        }
        FVector2D AlignmentVec(0.f, 0.f);
        EnemyHealthBar->SetAlignmentInViewport(AlignmentVec);
    }
      if(WPauseMenu)
    {
        PauseMenu = CreateWidget<UUserWidget>(this, WPauseMenu);
        if(PauseMenu)
        {
            PauseMenu->AddToViewport();
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);
        }
    }

}

void AMainPlayerController::Tick(float DeltaTime) 
{
    Super::Tick(DeltaTime);

    
    
    if(EnemyHealthBar)
    {
        FVector2D PositionInViewport;
        
        ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);

        PositionInViewport.Y -= 250.f;
        PositionInViewport.X -= 125.f;

        FVector2D SizeInViewport(200.f,75.f);

        EnemyHealthBar->SetPositionInViewport(PositionInViewport);
        EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewport);

    
    }

}

void AMainPlayerController::DisplayEnemyHealthBar()
{
    if(EnemyHealthBar)
    {
        bEnemyHealthBarVisible=true;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMainPlayerController::RemoveEnemyHealthBar()
{
if(EnemyHealthBar)
    {
        bEnemyHealthBarVisible=false;
        EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMainPlayerController::DisplayPauseMenu_Implementation()
{
    if(PauseMenu)
        {
            bPauseMenuVisible=true;
            PauseMenu->SetVisibility(ESlateVisibility::Visible);

            FInputModeGameAndUI InputMode;
            
            SetInputMode(InputMode);
            bShowMouseCursor=true;

            
        }
}

void AMainPlayerController::RemovePauseMenu_Implementation()
{
    if(PauseMenu)
        {
            bPauseMenuVisible=false;
              bShowMouseCursor=false;
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);

            FInputModeGameOnly InputMode;
            
            SetInputMode(InputMode);
            
        }
}

void AMainPlayerController::TogglePause()
{
	if(bPauseMenuVisible)
    {
        RemovePauseMenu();
        bPauseMenuVisible=false;
      
    }
    else
    {
        DisplayPauseMenu();
        bPauseMenuVisible=true;
    }
}