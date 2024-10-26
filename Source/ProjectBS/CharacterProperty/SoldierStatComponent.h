// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/SoldierStatData.h"
#include "SoldierStatComponent.generated.h"

DECLARE_DELEGATE (FDynamicDeadDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTBS_API USoldierStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USoldierStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	  void SetStat(const FSoldierStatData& statData);
	FORCEINLINE float GetDetectRange(){return  BaseStatData.SightRange;}
	FORCEINLINE float GetAttackRange(){return  BaseStatData.AttackRange;}
	FORCEINLINE float GetAttackSpeed(){return  BaseStatData.AttackSpeed;}
	FORCEINLINE float GetAttackDamange(){return BaseStatData.AttackDamage;}
	FORCEINLINE float GetMovementSpeed(){return  BaseStatData.MovementSpeed;}
	FORCEINLINE const FSoldierStatData& GetStat(){return BaseStatData;}
public:

	void SetHp(float newHp);
	
	void ApplyDamage(float Damage);

public:
	FDynamicDeadDelegate FOnDeadEvent;

protected:
	//스텟컴포넌트의 속성이 저장이 될때 디스크에 저장이 된다.
	//hp 경우 매 앱을 시작할때마다 초기화가 되어야하니 저장필요 없다. 이떄 Transient 사용함
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY()
	FSoldierStatData BaseStatData;

};
