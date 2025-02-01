// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleBatchUI.h"

#include "ProjectBSGameMode.h"
#include "TitleUI.h"
#include "BattleBatchUI/BattleCardDragDropOperation.h"
#include <Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include "Blueprint/WidgetBlueprintLibrary.h"
// #include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "GameMode/BSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameDataManager.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"
#include "UI/BatchUnitItemSlot.h"
#include "Components/ScrollBox.h"
#include "UI/BattleBatchUI/BattleCardItem.h"
#include "GameData/SoldierCharData.h"



void UBattleBatchUI::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Start->OnClicked.AddDynamic(this, &UBattleBatchUI::StartButtonClicked);

	Button_Back->OnClicked.AddDynamic(this, &UBattleBatchUI::OnClickButtonBack);

	// TextBox_Num->OnTextCommitted.AddDynamic(this,&UBattleBatchUI::TextBoxTextChanged);

	UBSGameInstance* gameIns = Cast<UBSGameInstance>(GetWorld()->GetGameInstance()) ;
	if(gameIns != nullptr)
	{
		gameIns->OnBattleCostChanged.AddUObject(this,&UBattleBatchUI::BattleCostChanged);
	}
		
	// TextBox_Num->SetText(FText::FromString(FString::FromInt(ForceCount)));


	// UUniformGridPanel* uniformGridPanel = Cast<UUniformGridPanel>(SoldierUnitGridPanel);
	// int colum = 0;
	// int row = 0;
	// for (int i = 0; i < 12; ++i)
	// {
	// 	colum = i % 4;
	// 	row = i / 4;
	// 	UBatchUnitItemSlot* newSlot = CreateWidget<UBatchUnitItemSlot>(GetWorld(),SoldierUnitGridSlotWidgetClass);
	// 	const FSoldierCharData* charDataPtr =  UGameDataManager::Get()->GetSoldierCharData(i+1);
	// 	FSoldierCharData data = charDataPtr != nullptr? *charDataPtr : FSoldierCharData();
	// 	newSlot->SetSlot(data);
	// 	uniformGridPanel->AddChildToUniformGrid(newSlot,row,colum);
	// }
	//
	// //현재 스테이지에 적병력정보
	// if(gameIns)
	// {
	// 	TTuple<TArray<FString>,TArray<int32>> enemiesNumData = UGameDataManager::Get()->GetSoldierNumsInfoInLevelStage(gameIns->GetGameLevelId());
	//
	// 	TArray<FString> names = enemiesNumData.Get<0>();
	// 	TArray<int32> nums = enemiesNumData.Get<1>();
	//
	// 	FString str;
	// 	
	// 	for (int i = 0; i < names.Num(); ++i)
	// 	{
	// 		str.Appendf(TEXT("%s %d"),*names[i],nums[i]);
	// 		str.Append(TEXT(" "));
	// 	}
	//
	// 	Text_NextEnemies->SetText(FText::FromString(str));
	// }

	TArray<FSoldierCharData> ownSoldiers = UGameDataManager::Get()->GetAllOwnSoldierCharData();

	for (  auto soldierCharData : ownSoldiers)
	{
		AddScrollItemToScrollBox(&soldierCharData);
	}
	
	
}

bool UBattleBatchUI::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	// Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	
	
    UBattleCardDragDropOperation* Operation = Cast<UBattleCardDragDropOperation>(InOperation);
    if(Operation)
    {
        UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop !!"));

        // (1) 드래그 앤 드롭 이벤트에서 Slate 좌표 획득
        // FVector2D ScreenPos = InDragDropEvent.GetScreenSpacePosition();

    	
        // (2) DPI 스케일 보정
        float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);

    	// 1) UWidgetLayoutLibrary::GetMousePositionOnViewport 로부터 뷰포트상 마우스 좌표 획득
    	FVector2D MousePosOnViewport = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
    		UE_LOG(LogTemp, Warning, TEXT("Mouse Pos On Viewport: %s"), *MousePosOnViewport.ToString());

    	FVector2D CorrectedScreenPos = MousePosOnViewport * ViewportScale;

    	
        // (3) PlayerController에서 Deproject
        APlayerController* PC = GetOwningPlayer();
        if (PC)
        {
            // 혹시 뷰포트 크기도 찍어본다.
            int32 SizeX, SizeY;
            PC->GetViewportSize(SizeX, SizeY);
            UE_LOG(LogTemp, Warning, TEXT("Viewport Size: %d x %d"), SizeX, SizeY);

            FVector WorldLocation, WorldDirection;
            PC->DeprojectScreenPositionToWorld(CorrectedScreenPos.X, CorrectedScreenPos.Y, WorldLocation, WorldDirection);

            // UE_LOG(LogTemp, Warning, TEXT("ScreenPos:%s, mousePos : %s (Corrected:%s) => WorldLoc:%s, Dir:%s"),
            //        *ScreenPos.ToString(),
            //        *MousePosOnViewport.ToString(),
            //        *CorrectedScreenPos.ToString(),
            //        *WorldLocation.ToString(),
            //        *WorldDirection.ToString());

            // (4) 라인트레이스로 지면 충돌 위치 얻기
            FHitResult HitResult;
            FVector TraceStart = WorldLocation;
            FVector TraceEnd   = WorldLocation + (WorldDirection * 100000.f);

        	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.f, 0, 2.f);

        	
            if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
            {
                FVector DropWorldPos = HitResult.Location;
                UE_LOG(LogTemp, Warning, TEXT("드롭 위치의 월드 좌표: %s"), *DropWorldPos.ToString());

				UGameFieldManager::Get(this)->SetTargetBatchSoliderCharId(Operation->CharId);
            	UGameFieldManager::Get(this)->BatchSoldier(DropWorldPos,ETeamType::OwnTeam);
            	
                // 디버그
                // DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, 2.f, 0, 2.f);
                // DrawDebugSphere(GetWorld(), DropWorldPos, 25.f, 12, FColor::Red, false, 2.f);
            }
        }


        return true;
    }
    return false;
}

void UBattleBatchUI::AddScrollItemToScrollBox( FSoldierCharData* charData)
{

	if(BattleCardScrollBox == nullptr )
		return;

	UBattleCardItem* newScrollItemWidget =  CreateWidget<UBattleCardItem>(GetWorld(),CardScrollItemClass);

	if(newScrollItemWidget == nullptr)
		return;

	newScrollItemWidget->SetItem(charData->Id);
	// newScrollItemWidget->SetDesiredSizeInViewport(FVector2D(357, 513));
	// newScrollItemWidget->SetItem(ChapterId,stageData);
	// newScrollItemWidget->SetVisibility( ESlateVisibility::Visible);

	BattleCardScrollBox->AddChild( newScrollItemWidget);
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


void UBattleBatchUI::OnClickButtonBack()
{
	//타이틀 UI 노출
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController!=nullptr)
	{
		UUIManager::Get()->AddUI(TEXT("StageSelectUI"),playerController);

		UUIManager::Get()->RemoveUI(this);
	}
}
