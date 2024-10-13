// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

USoundManager* USoundManager::Instance = nullptr;

USoundManager* USoundManager::Get()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<USoundManager>();
		Instance->AddToRoot();
	}

	return Instance;
}

USoundManager* USoundManager::GetSoundManagerInstance()
{
	return Get();
}

void USoundManager::PlaySound2DCustom(USoundBase* sound,UGameInstance* WorldContextObject)
{
	UE_LOG(LogTemp, Warning, TEXT("Playing Sound: %s"), *sound->GetName());
	// UGameplayStatics::PlaySound2D(WorldContextObject, sound);
	UAudioComponent* audioComponent = UGameplayStatics::SpawnSound2D(WorldContextObject, sound,1.f,1.f,0.f,nullptr,true,false);
	if(audioComponent)
	{
		audioComponent->bAutoDestroy = false;
		audioComponent->OnAudioFinishedNative.AddUObject(this,&USoundManager::OnSoundFinished);
		// audioComponent->bAllowSpatialization = false
		// audioComponent->set
	}
}

void USoundManager::OnSoundFinished(UAudioComponent* AudioComponent)
{
	if (AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sound Finished Playing!"));

		// 컴포넌트 수동 파괴
		AudioComponent->DestroyComponent();
	}
}
