#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include  "SoldierCharData.generated.h"

USTRUCT(BlueprintType)
struct FSoldierCharData : public FTableRowBase
{

	GENERATED_BODY();
    
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	int Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	int StatId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	FString UnitName;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	FString ResourceName; 
};