// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BatchUnitItemSlot.h"

#include "Components/Button.h"

void UBatchUnitItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Slot->OnClicked.AddDynamic(this, &UBatchUnitItemSlot::SlotButtonClicked);
}

void UBatchUnitItemSlot::SlotButtonClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("SlotButtonClicked %s"), *GetName());
}
