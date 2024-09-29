// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/SoldierCharData.h"
#include "BatchUnitItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBatchUnitItemSlot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void  SlotButtonClicked();

protected:
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Slot;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Item;

protected:

	uint32 Id;
	
public:
	void SetSlot(const FSoldierCharData charTable);	
};
