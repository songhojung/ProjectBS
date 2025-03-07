// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BattleBatchUI/BattleCardItem.h"

#include "UI/BattleBatchUI/BattleCardDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameData/SoldierCharData.h"
#include "Manager/GameDataManager.h"
#include "Manager/GameFieldManager.h"



void UBattleCardItem::NativeConstruct()
{
	Super::NativeConstruct();
}


void UBattleCardItem::SetItem(int charId)
{
	CharId = charId;

	const FSoldierCharData* charData = UGameDataManager::Get()->GetSoldierCharData(charId);
	
	if(charData == nullptr)
		return;
	
	const FSoldierStatData* charStatData = UGameDataManager::Get()->GetSoldierStatData(charData->StatId);

	if(charStatData == nullptr)
		return;
	
	///Script/Engine.Texture2D'/Game/UIResource/card_Fox.card_Fox'
	FString TexturePath = FString::Printf(TEXT("/Game/UIResource/card_%s.card_%s"), *charData->UnitName, *charData->UnitName);
	UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, *TexturePath);

	//캐릭터이미지
	if (NewTexture && Image_CharCard)
		Image_CharCard->SetBrushFromTexture(NewTexture);

	//캐릭터 이름
	Text_Name->SetText(FText::FromString(charData->UnitName));

	//캐릭터 비용
	FString costStr = FString::Printf(TEXT("%d"),charData->Cost);
	Text_Cost->SetText(FText::FromString(costStr));

	//캐릭터공격력
	int32 atk = FMath::FloorToInt(charStatData->AttackDamage); // 소수점 아래를 버림
	FString atkStr = FString::Printf(TEXT("%d"),atk);
	Text_Attack->SetText(FText::FromString(atkStr));
	
	//캐릭터최대체력
	int32 hpInt = FMath::FloorToInt(charStatData->MaxHp); // 소수점 아래를 버림
	FString hpStr = FString::Printf(TEXT("%d"),hpInt);
	Text_Hp->SetText(FText::FromString(hpStr));
}


FReply UBattleCardItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent,this,EKeys::LeftMouseButton).NativeReply;
	}
		
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UBattleCardItem::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr)
	{
		// 드래그 슬롯을 생성합니다.
		UBattleCardDragDropOperation* Operation = NewObject<UBattleCardDragDropOperation>();
		Operation->CharId = CharId;
		OutOperation = Operation;

		// Drag 위젯을 생성합니다.
		if (DragWidgetClass)
		{
			UBattleCardItem* DragWidget = CreateWidget<UBattleCardItem>(GetWorld(), DragWidgetClass);
			if (DragWidget)
			{
				// 생성된 위젯을 초기화해줍니다.
				// DragWidget->SlotType = SlotType;
				// DragWidget->SetOwningActor(OwningActor);
				// DragWidget->SlotIndex = SlotIndex;
				// DragWidget->Init();
				DragWidget->SetItem(Operation->CharId);

				// 드래그 슬롯의 드래그 위젯을 설정합니다.
				Operation->DefaultDragVisual = DragWidget;
			}
		}
	}
}

// bool UBattleCardItem::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,UDragDropOperation* InOperation)
// {
// 	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
// 	UBattleCardDragDropOperation* Operation = Cast<UBattleCardDragDropOperation>(InOperation);
// 	if(Operation)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("NativeOnDrop !!"));
//
// 		// 1) 드래그가 떨어진 지점의 스크린 좌표(screen position) 획득
// 		FVector2D ScreenPosition = InDragDropEvent.GetScreenSpacePosition();
//
// 		// 2) PlayerController를 통해 스크린 좌표 → 월드 좌표/방향 으로 디프로젝션
// 		if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
// 		{
// 			FVector WorldLocation, WorldDirection;
// 			PC->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection);
//
// 			UE_LOG(LogTemp, Warning, TEXT("Deprojected WorldLocation: %s, Direction: %s"),*WorldLocation.ToString(), *WorldDirection.ToString());
//
// 			UGameFieldManager::Get(this)->SetTargetBatchSoliderCharId(CharId);
// 			UGameFieldManager::Get(this)->BatchSoldier(WorldLocation,ETeamType::OwnTeam);
// 			
// 			// // 3) 필요하다면 라인 트레이스(Line Trace)로 실제 충돌 위치를 찾는다.
// 			// //    가령 캐릭터가 서있는 지면 등과의 충돌 지점을 구할 수 있다.
// 			// FHitResult HitResult;
// 			// FVector TraceStart = WorldLocation;
// 			// FVector TraceEnd   = WorldLocation + (WorldDirection * 10000.0f); // 임의 거리
// 			//
// 			// FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(DragDropTrace), true, PC->GetPawn());
// 			// // → 필요 시 Pawn 등을 무시하거나, 다른 채널을 사용하도록 설정
// 			//
// 			// if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
// 			// {
// 			// 	// 라인 트레이스가 충돌한 곳의 월드 좌표
// 			// 	UE_LOG(LogTemp, Warning, TEXT("Hit location: %s"), *HitResult.Location.ToString());
//    //              
// 			// 	// 여기서 드래그된 오브젝트를 소환/배치하거나, 다른 로직을 수행
// 			// }
// 		}
// 		
// 		return true;
// 	}
//
// 	return false;
// }

void UBattleCardItem::ClickedButtonItem()
{
}
