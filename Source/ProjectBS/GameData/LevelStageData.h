#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include  "LevelStageData.generated.h"

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FLevelStageData : public FTableRowBase
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	int32 Id;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	FString StageName;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	int32 OwnTeamMaxCost;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	TArray<int32> OwnTeamBatchGridSize;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	TArray<int32> EnemyTeamBatchGridSize;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	int32 EnemyTeamCharNum;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	TArray<int32> EnemyTeamBatchCharIds;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	TArray<int32> EnemyTeamBatchPositions;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	bool EvnFlag;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	int32 EvnId;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	FString EvnName;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	double EvnSize;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	double AddMoveSpeedValue;

	/** Please add a variable description */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="LevelStage")
	double AddAttackSpeedValue;
};
