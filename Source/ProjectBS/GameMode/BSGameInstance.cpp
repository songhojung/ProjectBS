// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BSGameInstance.h"

#include "BatchGrid/BatchGridActor.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/GameDataManager.h"
#include "Manager/GameFieldManager.h"
#include "Manager/UIManager.h"
#include "UI/BattleEndUI.h"

class UBattleEndUI;

UBSGameInstance::UBSGameInstance()
{
	bNeedLevelLoadedProcess = false;
	
	bGameStarted = false;
	bBattleStarted = false;
	bBattleEnd = false;

	ClearBattleCost();
	SetGameLevelId(1);
}

void UBSGameInstance::Init()
{
	Super::Init();
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this,&UBSGameInstance::OnPostLoadMap);
}

void UBSGameInstance::OnPostLoadMap(UWorld* loadedWorld)
{
	//게임레벨 로드음 해당레벨의 이후 처리진행
	PostGameLevelLoaded();
}

int32 UBSGameInstance::GetMaxBattleCost()
{
	const FLevelStageData* levelData = UGameDataManager::Get()->GetLevelStageData(GameLevelId);
	if(levelData != nullptr )
		return  levelData->OwnTeamMaxCost;

	return 0;
}

bool UBSGameInstance::IsEnoughBattleCost( int charId)
{
	const FLevelStageData* levelData = UGameDataManager::Get()->GetLevelStageData(GameLevelId);

	const FSoldierCharData* charData = UGameDataManager::Get()->GetSoldierCharData(charId);
	
	if(levelData != nullptr && charData != nullptr)
	{
		int32 maxValue = levelData->OwnTeamMaxCost;

		int32 value = UsedBattleCost + charData->Cost;

		if(value <= maxValue)
			return true;
	}
	
	return false;
}

bool UBSGameInstance::CheckGameLevelId(int32 gameLevelId)
{
	const FLevelStageData* levelData =  UGameDataManager::Get()->GetLevelStageData(gameLevelId);
	return levelData !=nullptr;
}


void UBSGameInstance::GameStart(int32 gameLevelId)
{
	//레벨에 맞는 레벨로드
	bool isChangedGameLevel = false;
	LoadLevel(gameLevelId , isChangedGameLevel);

	bNeedLevelLoadedProcess = true;
	
	// if(isChangedGameLevel == false)
	// {
	// 	UWorld* world = GetWorld();
	// 	
	// 	PostGameLevelLoaded();
	// }
	
}

void UBSGameInstance::PostGameLevelLoaded()
{
	if(bNeedLevelLoadedProcess == false)
		return;


	
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();

	if(playerController)
	{
		// UUIManager::Get()->AddUI(TEXT("TitleUI"),playerController);
		//배치 UI 노출
		UUIManager::Get()->AddUI(TEXT("BattleBatchUI"),playerController);


		
		//병력배치비용 다시 초기화
		ClearBattleCost();
		
		//이전 필드 요소들 클리어
		UGameFieldManager::Get(this)->ClearFieldComponents();
		
		UGameFieldManager::Get(this)->SetSpawnAreas();
		
		
		//필드에 배치 grid actor 생성
		UGameFieldManager::Get(this)->ReadyForBatch();
	}

	bBattleStarted = false;

	bNeedLevelLoadedProcess = false;
}

void UBSGameInstance::BattleStart(int32 count)
{
	bBattleEnd = false;

	bBattleStarted = true;
 
	
	// 게임필드 에 필드에 병력, 오브젝트 등 로드.
	UGameFieldManager* gfm = UGameFieldManager::Get(this);
	gfm->StartBattleInField(GameLevelId);

	//배치 그리드 비활성
	if(gfm->GetBatchGrid())
		gfm->GetBatchGrid()->SetActiveBatchGrid(false);

	//팀별 유닛 갯수 저장
	int32 ownTeamUnitCount = gfm->GetTeamSoldierArray(ETeamType::OwnTeam).Num();
	int32 enemyTeamUnitCount = gfm->GetTeamSoldierArray(ETeamType::EnemyTeam).Num();

	UnitsByTeamMap.Empty();
	UnitsByTeamMap.Add(ETeamType::OwnTeam,ownTeamUnitCount);
	UnitsByTeamMap.Add(ETeamType::EnemyTeam,enemyTeamUnitCount);
}

void UBSGameInstance::OnDeadUnit(ETeamType teamType, int32 count)
{
	if(UnitsByTeamMap.Contains(teamType))
	{
		UnitsByTeamMap[teamType] -= count;

		if(UnitsByTeamMap[teamType] <= 0)
		{
			BattleEnd();
		}
	}
}

void UBSGameInstance::BattleEnd()
{
	if(bBattleEnd)
		return;
	
	bBattleEnd = true;
	
	UGameFieldManager* gfm = UGameFieldManager::Get(this);
	TArray<ASoldierBaseCharacter*> allUnits;
	TArray<ASoldierBaseCharacter*> ownerSoldiers = gfm->GetTeamSoldierArray(ETeamType::OwnTeam);
	TArray<ASoldierBaseCharacter*> enemySoldiers = gfm->GetTeamSoldierArray(ETeamType::EnemyTeam);
	allUnits.Append(ownerSoldiers);
	allUnits.Append(enemySoldiers);
	
	//모든병력 행동중지
	for (auto unit : allUnits)
	{
		if(unit==nullptr)
			continue;
		
		if(unit->IsDead()==false)
			unit->SetStop();
	}


	//승리팀 판별
	ETeamType winTeam = ETeamType::NeutralTeam;
	for (auto team : UnitsByTeamMap)
	{
		if(team.Value > 0)
		{
			winTeam = team.Key;
			break;
		}
	}

	//승리팀 캐싱
	BattleWinTeam = winTeam;
	
	FString teamStr = StaticEnum<ETeamType>()->GetValueAsString(winTeam);
	UE_LOG(LogTemp, Warning, TEXT("********************* BattleEnd winTeam is %s ***********************"),*teamStr );



	//결과UI 노출
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if(playerController!=nullptr)
	{
		UUIManager::Get()->AddUI(TEXT("BattleEndUI"),playerController,FCompletedAddUIDelegate::CreateLambda([&](UUserWidget* widget)
		{
			UBattleEndUI* battleEndUI = Cast<UBattleEndUI>(widget);
			if(battleEndUI)
				battleEndUI->SetUI(BattleWinTeam);
		}));
	}
}

void UBSGameInstance::LoadLevel(int32 gameLevelId, bool& outChangedLevel)
{
	outChangedLevel = false;
	
	const FLevelStageData* levelData = UGameDataManager::Get()->GetLevelStageData(gameLevelId);
	if(levelData ==nullptr)
		return;
	
	UWorld* world = GetWorld();
	if (world)
	{
		// 현재 레벨 이름 얻기
		FString CurrentLevelName = FPackageName::GetShortName(world->GetMapName());

		// PIE 접두사 제거 (에디터에서만 유효, 실제 빌드에서는 이 코드가 영향 없음)
		FString PIEPrefix = world->StreamingLevelsPrefix; // PIE 접두사 (e.g., "UEDPIE_0_")
		if (CurrentLevelName.StartsWith(PIEPrefix))
		{
			//prefix길이만큼 자르고 오른쪽 문자열만 가지고옴
			CurrentLevelName = CurrentLevelName.RightChop(PIEPrefix.Len());
		}

		if(CurrentLevelName.Equals(levelData->StageName)==false)
		{

			FString loadedLevelName = /*"/Game/Maps/" + */levelData->StageName;
			UGameplayStatics::OpenLevel(world, FName(*loadedLevelName));

			outChangedLevel = true;
		}
	}
}
