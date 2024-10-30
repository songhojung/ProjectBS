// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/SoldierAIController.h"
#include "CharacterProperty/SoldierStatComponent.h"
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

	virtual void PostInitializeComponents() override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//teamSection
public:
	void SetTeam(ETeamType Team);
	void SetStat(const FSoldierStatData& statData);
	virtual ETeamType GetTeam() const override;
	virtual USoldierStatComponent* GetStatComponent(){return StatComponent;}
	virtual AActor& GetOtherTeamBaseActor() const override;
	FORCEINLINE class ASoldierAIController* GetAIController() {return Cast<class ASoldierAIController>(GetController());}
	virtual float GetAIDectectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAIAttackSpeed() override;
	virtual float GetAIMovementSpeed() override;
	virtual float GetAIColliderRadius() override;
	float GetAttackAnimLength();

	virtual  bool IsDead() override;
	//AttackSection
public:
	void SetCollisionProfileName(ETeamType team);
	virtual void Attack() override;

	void virtual AttackHitCheck() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;

	void SetDead();

	void SetStop();

	void ForceFront();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<class USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> AttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class UTeamComponent> TeamComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class USoldierStatComponent> StatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Team)
	TObjectPtr<class UPhysicalAnimationComponent> PhysicalAnimation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit)
	TObjectPtr<class USceneComponent> HitPointStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit)
	TObjectPtr<class USceneComponent> HitPointEnd;

protected:
	uint8 bIsDead : 1;

};
