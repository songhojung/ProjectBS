// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleBatchUI.h"

#include "ProjectBSGameMode.h"
#include "TitleUI.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameDataManager.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"
#include "UI/BatchUnitItemSlot.h"

void UBattleBatchUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Start->OnClicked.AddDynamic(this, &UBattleBatchUI::StartButtonClicked);

	// TextBox_Num->OnTextCommitted.AddDynamic(this,&UBattleBatchUI::TextBoxTextChanged);

	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetWorld()->GetGameInstance()) ;
	if(gameIns != nullptr)
	{
		gameIns->OnBattleCostChanged.AddUObject(this,&UBattleBatchUI::BattleCostChanged);
	}
		
	// TextBox_Num->SetText(FText::FromString(FString::FromInt(ForceCount)));


	UUniformGridPanel* uniformGridPanel = Cast<UUniformGridPanel>(SoldierUnitGridPanel);
	int colum = 0;
	int row = 0;
	for (int i = 0; i < 12; ++i)
	{
		colum = i % 4;
		row = i / 4;
		UBatchUnitItemSlot* newSlot = CreateWidget<UBatchUnitItemSlot>(GetWorld(),SoldierUnitGridSlotWidgetClass);
		const FSoldierCharData* charDataPtr =  UGameDataManager::Get()->GetSoldierCharData(i+1);
		FSoldierCharData data = charDataPtr != nullptr? *charDataPtr : FSoldierCharData();
		newSlot->SetSlot(data);
		uniformGridPanel->AddChildToUniformGrid(newSlot,row,colum);
	}

	//현재 스테이지에 적병력정보
	if(gameIns)
	{
		TTuple<TArray<FString>,TArray<int32>> enemiesNumData = UGameDataManager::Get()->GetSoldierNumsInfoInLevelStage(gameIns->GetGameLevelId());

		TArray<FString> names = enemiesNumData.Get<0>();
		TArray<int32> nums = enemiesNumData.Get<1>();

		FString str;
		
		for (int i = 0; i < names.Num(); ++i)
		{
			str.Appendf(TEXT("%s %d"),*names[i],nums[i]);
			str.Append(TEXT(" "));
		}

		Text_NextEnemies->SetText(FText::FromString(str));
	}

	
}

void UBattleBatchUI::StartButtonClicked()
{



	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetGameInstance());
	if(gameIns)
		gameIns->BattleStart(ForceCount);
	

	UUIManager::Get()->RemoveUI(this);
}

void UBattleBatchUI::TextBoxTextChanged(const FText& Text, ETextCommit::Type CommitMethod)
{
	FString StringValue = Text.ToString();  // FText -> FString로 변환하여 저장
	ForceCount = FCString::Atoi(*StringValue);
}

void UBattleBatchUI::BatchUnitItemClicked(int32 id)
{

	UGameFieldManager::Get(this)->ChangeSampleBatchSoldier(id);
	UGameFieldManager::Get(this)->SetTargetBatchSoliderCharId(id);
}

void UBattleBatchUI::BattleCostChanged(int32 currentCost, int32 maxCost)
{
	FString costStr = FString::Printf(TEXT("%d/%d"),currentCost,maxCost);
	Text_BattleCost->SetText(FText::FromString(costStr));
}
