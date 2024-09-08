// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoldierStatComponent.generated.h"


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
	 void SetStat(float hp, float attackDamage);
	FORCEINLINE float GetDetectRange(){return  700.f;}
	FORCEINLINE float GetAttackDamange(){return AttackDamage;}
public:

	void ApplyDamage(float Damage);

protected:
	//스텟컴포넌트의 속성이 저장이 될때 디스크에 저장이 된다.
	//hp 경우 매 앱을 시작할때마다 초기화가 되어야하니 저장필요 없다. 이떄 Transient 사용함
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float AttackDamage;

};
