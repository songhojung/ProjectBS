// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BatchGridActor.generated.h"

UCLASS()
class PROJECTBS_API ABatchGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABatchGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	TObjectPtr<class UProceduralMeshComponent> ProceduralMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	TObjectPtr<class UProceduralMeshComponent> SelectionProceduralMesh;

	UPROPERTY(visibleAnywhere,BlueprintReadOnly,Category="Mesh")
	TObjectPtr<class UMaterial> GridMaterial;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	int32 NumRows;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	int32 NumColumns;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	float TileSize;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	float LineThickness;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	FColor LineColor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	FColor SelectionEnableColor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	FColor SelectionDisableColor;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	float LineOpacity;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Grid")
	float SelectionOpacity;

protected:
	int32 SoldierCharIdOnGrid; // 그리드에 배치할 병사 id
	int32 LastSelectionRow;
	int32 LastSelectionColumn;

	UMaterialInstanceDynamic* selectionEnableMatInstanceDynamic ;
	UMaterialInstanceDynamic* selectionDisableMatInstanceDynamic ;

public:
	FORCEINLINE int32 GetNumRows() const { return NumRows; }
	FORCEINLINE int32 GetNumColumns() const { return NumColumns; }

	FORCEINLINE void SetSoldierIdOnGrid(int32 id) { SoldierCharIdOnGrid = id; } // 그리드에 배치할 병사 id 설정

protected:
	void CreateLine(FVector start, FVector end, float thickNess, TArray<FVector>& vertices, TArray<int32>& triangles);

	float GridWidth();
	float GridHeight();

	UMaterialInstanceDynamic* CreateMaterialInstance(FLinearColor color, float opacity);

public:
	void SetNumRowsAndColumes(int32 numRows, int32 numColumns);
	void MakeBatchGrid();
	bool LocationToTile(FVector location, int32& outRow, int32& outColumn);
	bool TileToGridLocation(int32 row, int32 column, bool isCenter, FVector2d& outGridLocation);
	void SetSelectedTile(int32 row, int32 column,bool isGreenSelectionColor);
	int32 GetGridIndex(int32 row, int32 column);
	bool TileValid(int32 row, int32 column);

	void SetActiveBatchGrid(bool isActive);

	

};
