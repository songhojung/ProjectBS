// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SoldierCharacterAIInterface.h"
#include "CharacterProperty/TeamComponent.h"
#include "SoldierBaseCharacter.generated.h"

enum class ETeamType : uint8;

UCLASS()
class PROJECTBS_API ASoldierBaseCharacter : public ACharacter ,public ISoldierCharacterAIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldierBaseCharacter();

public:
	FORCEINLINE UTeamComponent* GetTeamComponent() const { return TeamComponent; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//teamSection
	public:
	void SetTeam(ETeamType Team);


	//AttackSection
public:
	virtual void Attack() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class UTeamComponent> TeamComponent;
		

	

};
