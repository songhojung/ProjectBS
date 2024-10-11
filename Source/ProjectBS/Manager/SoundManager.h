// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API USoundManager : public UObject
{
	GENERATED_BODY()

	static USoundManager* Instance;

public:
	// Static instance getter
	static USoundManager* Get();


	UFUNCTION(BlueprintCallable, Category="Sound Management")
	static USoundManager* GetSoundManagerInstance();

public:
	UFUNCTION(BlueprintCallable, Category="UI Management")
	void PlaySound2DCustom(USoundBase* sound,UGameInstance* WorldContextObject);

	UFUNCTION()
	void OnSoundFinished(UAudioComponent* AudioComponent);
};
