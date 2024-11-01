// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterProperty/TeamComponent.h"
#include "SoldierCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USoldierCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTBS_API ISoldierCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual ETeamType GetTeam() const = 0;
	virtual AActor& GetOtherTeamBaseActor() const = 0;		//상대팀 진영 액터

	virtual  bool IsDead() = 0;
	// virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDectectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAIAttackSpeed() = 0;
	virtual float GetAIMovementSpeed() = 0;
	virtual float GetAIColliderRadius() = 0;
	virtual float GetAITurnSpeed() = 0;
	
	virtual void Attack() = 0;
};
