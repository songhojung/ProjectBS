// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterProperty/TeamComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Spawn/SpawnArea.h"

// Sets default values for this component's properties
UTeamComponent::UTeamComponent()
{
	ConstructorHelpers::FObjectFinder<UMaterialInstance> ownteamMatInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/ProjectBS/Material/MatIns_SoldierOwnTeamColor.MatIns_SoldierOwnTeamColor'"));
	if(ownteamMatInstanceRef.Object)
		TeamMaterials.Add(ownteamMatInstanceRef.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInstance> enemyTeamMatInstanceRef(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/ProjectBS/Material/MatIns_SoldierEnenyTeamColor.MatIns_SoldierEnenyTeamColor'"));
	if(enemyTeamMatInstanceRef.Object)
		TeamMaterials.Add(enemyTeamMatInstanceRef.Object);

	
}


// Called when the game starts
void UTeamComponent::BeginPlay()
{
	Super::BeginPlay();


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnArea::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ASpawnArea* SpawnArea = Cast<ASpawnArea>(Actor);
		if(SpawnArea!= nullptr && SpawnArea->GetTeamType() != TeamType)
		{
			OtherTeamBaseActor = Actor;

			
			FString otherTeamStr = StaticEnum<ETeamType>()->GetValueAsString(SpawnArea->GetTeamType());
			UE_LOG(LogTemp, Warning, TEXT("@@@@Find Enemy Team Spawn : %s / %s / %s"), *GetName(), *Actor->GetName(), *otherTeamStr);
		}
			
	}
}

void UTeamComponent::SetTeamDynamicMaterialInstanceOverride(USkeletalMeshComponent* mesh,ETeamType teamType)
{
	mesh->SetMaterial(0,TeamMaterials[(int8)teamType]);
}



