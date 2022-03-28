// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MainChar.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
    if (CharPawn==nullptr)
    {
        CharPawn=TryGetPawnOwner();
        if(CharPawn)
        {
            MainChar = Cast<AMainChar>(CharPawn);
        }
    }
    

}


/** Used for the Animation blueprint, sets the movement speed
 * by getting the player's velocity and sets our variable IsInAir to 
 * the same as the engine's falling check. These are used
 * to update our animation blueprint.
 */
void UMainAnimInstance::UpdateAnimationProperties()
{
    if (CharPawn==nullptr)
        {
            CharPawn=TryGetPawnOwner();
        }

    if(CharPawn)
    {
    
        FVector Speed = CharPawn->GetVelocity();
        FVector LateralSpeed = {Speed.X, Speed.Y, 0};
        MovementSpeed = LateralSpeed.Size();
        FTransform CharTransform = CharPawn->GetActorTransform();
        MovementAngle = CalculateDirection(Speed, CharTransform.Rotator());
//** UE_LOG(LogTemp,Warning, TEXT("%f"), MovementAngle); **//
        if(MainChar == nullptr)
        {
        MainChar = Cast<AMainChar>(CharPawn);
        }
        if(MainChar)
        {
           
           bJumpJustPressed = MainChar->bJustJumped;
           if(MainChar->bFPPerspective)
           {
               bLockedOnPerspectiveForAnims=true;
           }
           else
           {
               bLockedOnPerspectiveForAnims = MainChar->bLockedOnPerspective;
           }
        }

        
    

	    TurnAtValue = CharPawn->GetInputAxisValue("TurnRate");
        TurnAtValueTwo = CharPawn->GetInputAxisValue("Turn");
    
     
        // TurnAtValue = 0.f;
        // TurnAtValueTwo = 0.f;

        // UE_LOG(LogTemp, Warning, TEXT("TRUE!! Turn At Rate : %f"), TurnAtValue);
	    // UE_LOG(LogTemp, Warning, TEXT("TRUE!! Turn At Two Rate : %f"), TurnAtValueTwo);
   
    
                
        bIsInAir= CharPawn->GetMovementComponent()->IsFalling();
        bMyCrouch = CharPawn->GetMovementComponent()->IsCrouching();
        

        if(MovementSpeed >= 450.f)
        {
            bMySprint = true;
        }
        else
        {
            bMySprint=false;
        }

    }
}






