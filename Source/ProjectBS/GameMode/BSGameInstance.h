// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UBSGameInstance();

	virtual void Init() override;
};
