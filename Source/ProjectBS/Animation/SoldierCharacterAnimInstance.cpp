// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SoldierCharacterAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USoldierCharacterAnimInstance::USoldierCharacterAnimInstance()
{
	MovingThreshould = 3.0f;

}

void USoldierCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwenrCharacter = Cast<ACharacter>(GetOwningActor());

	if(OwenrCharacter)
	{
		Movement = OwenrCharacter->GetCharacterMovement();
	}

	
}

void USoldierCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(Movement !=nullptr)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
	}
}
