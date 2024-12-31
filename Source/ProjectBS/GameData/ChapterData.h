#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include  "ChapterData.generated.h"

USTRUCT(BlueprintType)
struct FChapterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	int32 Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	FString ChapterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	TArray<int32> StageLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	FString ChapterThumbNail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	FString Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Chapter")
	FString Summary;
};
