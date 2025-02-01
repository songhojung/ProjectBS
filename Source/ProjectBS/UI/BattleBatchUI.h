// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/SoldierCharData.h"
#include "BattleBatchUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UBattleBatchUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
		
protected:
	int32 ForceCount = 1; //병력 숫자

protected:
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UScrollBox* BattleCardScrollBox;
	//
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Start;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Back;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_BattleCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Battle")
	TSubclassOf<class UBattleCardItem> CardScrollItemClass;

protected:
	void AddScrollItemToScrollBox( FSoldierCharData* charData);
	
public:
	UFUNCTION()
	void StartButtonClicked();

	UFUNCTION()
	void TextBoxTextChanged(const FText& Text, ETextCommit::Type CommitMethod);

	void BatchUnitItemClicked(int32 id);

	void BattleCostChanged(int32 currentCost, int32 maxCost);
	
	UFUNCTION()
	void OnClickButtonBack();
};
