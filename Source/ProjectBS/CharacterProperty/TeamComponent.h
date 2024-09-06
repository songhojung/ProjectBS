// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"


UENUM(BlueprintType)
enum class ETeamType : uint8
{
	OwnTeam = 0,
	EnemyTeam,
	NeutralTeam, //중립팀
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBS_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTeamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FORCEINLINE void SetTeamType(ETeamType teamType) { TeamType = teamType; }
	FORCEINLINE ETeamType GetTeamType() const { return TeamType; }

public:
	FORCEINLINE AActor& GetOtherTeamBaseActor() {return *OtherTeamBaseActor; }

protected:	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = TeamType)
	ETeamType TeamType;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = TeamMaterial)
	TArray<TObjectPtr<class UMaterialInstance>> TeamMaterials;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> OtherTeamBaseActor;

public:
	void SetTeamDynamicMaterialInstanceOverride(USkeletalMeshComponent* mesh, ETeamType teamType);
};
