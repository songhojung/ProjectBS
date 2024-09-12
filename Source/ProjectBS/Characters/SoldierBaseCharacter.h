// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/SoldierCharacterAIInterface.h"
#include "Interface/IAnimantionAttackInterface.h"
#include "CharacterProperty/TeamComponent.h"
#include "GameData/SoldierStatData.h"
#include "SoldierBaseCharacter.generated.h"

enum class ETeamType : uint8;

UCLASS()
class PROJECTBS_API ASoldierBaseCharacter : public ACharacter ,public ISoldierCharacterAIInterface ,public  IIAnimantionAttackInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldierBaseCharacter();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//teamSection
	public:
	void SetTeam(ETeamType Team);
	void SetStat(const FSoldierStatData& statData);
	virtual ETeamType GetTeam() const override;
	virtual AActor& GetOtherTeamBaseActor() const override;
	virtual float GetAIDectectRange() override;

	virtual  bool IsDead() override;
	//AttackSection
public:
	virtual void Attack() override;

	void virtual AttackHitCheck() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void SetDead();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class UTeamComponent> TeamComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class USoldierStatComponent> StatComponent;
		

protected:
	uint8 bIsDead : 1;

};
