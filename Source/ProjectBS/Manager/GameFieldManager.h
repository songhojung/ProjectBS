// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "Characters/SoldierBaseCharacter.h"
#include "GameFieldManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UGameFieldManager : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

	UGameFieldManager();
public:
	// Static function to simplify access to the subsystem (Singleton style)
	static UGameFieldManager* Get(UObject* WorldContextObject);
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UGameFieldManager, STATGROUP_Tickables); }

protected:
	const float DISTANCE_MIN_BATCH = 700.f;
	
protected:

	TArray<TWeakObjectPtr<class ASpawnArea>> SpawnAreaArray;
	
	UClass* SoldierClass = nullptr;

	bool CanTick = false;

	int32 LastHoverMouseBatchGridIndex = -1;

	int32 TargetBatchSoliderCharId = 1;		//유저가 선택한 배치할 병력 CharId
	
	TWeakObjectPtr<class APlayerController> PlayerController;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Batch Grid")
	TObjectPtr<class ABatchGridActor> BatchGrid;

	class ASoldierBaseCharacter* BatchGridSampleSoldier;

	TArray<ASoldierBaseCharacter*> OwnSoldierArray;
	TArray<ASoldierBaseCharacter*> OtherSoldierArray;

	TMap<int32,bool> OwnTeamBatchGridAssignedMap;
	
protected:
	void OnWorldBeginPlay();
	//선택한 임시 병사 배치그리드 위에  마우스 포인터 트랙킹 처리 (legacy)
	void TrackSampleSoldierMouseOnBatchGrid();
	//선택한 임시 병사 땅 위에  마우스 포인터 트랙킹 처리
	void TrackSampleSoldierMouseOnGround();
	UClass* LoadCharacterClass(int charTableId);
	UClass* LoadBatchGridClass();
	ASoldierBaseCharacter* CreateSoldier(int32 charId, FVector location, FRotator rotation, ETeamType teamType);
public:
	void SetSpawnAreas();

	//배치용 그리드 액터 생성 (현재 사용안함)
	void CreateBatchGridActor();

	void ReadyForBatch();

public:
	void StartBattleInField(int32 levelStageId);

	void BatchSoldier(FVector location,  ETeamType teamType);

	void ChangeSampleBatchSoldier(int32 charId);

	void DestroySampleBatchSoldier();

	void ClearFieldComponents();

	FORCEINLINE void SetTargetBatchSoliderCharId(int32 charId){TargetBatchSoliderCharId = charId;}

	FORCEINLINE int32 GetTargetBatchSoliderCharId(){return TargetBatchSoliderCharId;}
	
	class ASpawnArea* GetTeamSpawnArea(ETeamType teamType);
	
	class ABatchGridActor* GetBatchGrid();

	TArray<ASoldierBaseCharacter*> GetTeamSoldierArray(ETeamType teamType);

	FORCEINLINE bool IsContainGridIndex(int32 gridIndex) {return OwnTeamBatchGridAssignedMap.Contains(gridIndex);}

	void GetActorForceOwnTeamTestCode();
};
