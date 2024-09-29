// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BatchUnitItemSlot.h"

#include "BattleBatchUI.h"
#include "GameData/SoldierCharData.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"

void UBatchUnitItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Slot->OnClicked.AddDynamic(this, &UBatchUnitItemSlot::SlotButtonClicked);
}

void UBatchUnitItemSlot::SlotButtonClicked()
{
	UE_LOG(LogTemp,Warning,TEXT("SlotButtonClicked %s"), *GetName());

	if(Id > 0)
	{
		UBattleBatchUI* batchUI =  Cast<UBattleBatchUI>(UUIManager::Get()->GetUI("BattleBatchUI"));
		if(batchUI!=nullptr)
		{
			batchUI->BatchUnitItemClicked(Id);
		}

	}
}

void UBatchUnitItemSlot::SetSlot(const FSoldierCharData charData)
{
	if(charData.Id <= 0)
	{
		//빈슬롯
		return;
	}

	Id = charData.Id;
	
	FString TexturePath = FString::Printf(TEXT("/Game/CraftResourcesIcons/Textures/Tex_feather_%02d.Tex_feather_%02d"), charData.Id, charData.Id);
	UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);

	if (NewTexture && Image_Item)
	{
		// Brush를 생성하여 이미지를 설정
		Image_Item->SetBrushFromTexture(NewTexture);
	}
	
}
