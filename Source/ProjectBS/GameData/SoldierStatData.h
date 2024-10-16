#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include  "SoldierStatData.generated.h"

USTRUCT(BlueprintType)
struct FSoldierStatData : public FTableRowBase
{

    GENERATED_BODY();
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    int Id;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float MaxHp;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float AttackDamage;
 
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float AttackSpeed;
     
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Stat")
    float SightRange;

};