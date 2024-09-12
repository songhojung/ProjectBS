// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierStatComponent.h"


// Sets default values for this component's properties
USoldierStatComponent::USoldierStatComponent()
{

}


// Called when the game starts
void USoldierStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USoldierStatComponent::SetStat(const FSoldierStatData& statData)
{
	CurrentHp  = statData.MaxHp;

	SetHp(statData.MaxHp);
	
	BaseStatData = statData;
}

void USoldierStatComponent::SetHp(float newHp)
{
	CurrentHp = FMath::Clamp(newHp, 0, BaseStatData.MaxHp);
	
}

void USoldierStatComponent::ApplyDamage(float Damage)
{
	CurrentHp -= Damage;

	if(CurrentHp <= KINDA_SMALL_NUMBER)
	{
		//dead 처리
		//dead 콜백호출
		FOnDeadEvent.ExecuteIfBound();
	}
}



