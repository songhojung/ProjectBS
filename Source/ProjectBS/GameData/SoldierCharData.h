#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include  "SoldierCharData.generated.h"

enum CharType
{
	Char = 0,
	Monster =1
};
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
	int Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	FString UnitName;  

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	FString ResourceName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Char")
	int Type;
	
};