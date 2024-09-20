// Fill out your copyright notice in the Description page of Project Settings.

#include "BatchGrid/BatchGridActor.h"
#include "Engine/Engine.h"
#include "ProceduralMeshComponent.h"

// Sets default values
ABatchGridActor::ABatchGridActor()
{
	ConstructorHelpers::FObjectFinder<UMaterial> gridMatInstanceRef(TEXT("/Script/Engine.Material'/Game/ProjectBS/Material/M_Grid.M_Grid'"));
	if(gridMatInstanceRef.Object)
		GridMaterial = gridMatInstanceRef.Object;
	
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMeshComponent"));
	SelectionProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("SelectionProceduralMeshComponent"));
	// RootComponent  = ProceduralMesh;

	NumRows = 10;
	NumColumns = 10;
	TileSize = 200.f;
	LineThickness = 10.f;
	LineColor = FColor::Green;
	LineOpacity = 1.0f;
	SelectionColor = FColor::Red;
	SelectionOpacity = 0.25f;
	
	LastSelectionRow = -1;
	LastSelectionColumn = -1;
	
	
	// UMaterialInstanceDynamic* lineMatInstanceDynamic =  CreateMaterialInstance(LineColor,LineOpacity);
	//
	// UMaterialInstanceDynamic* selectionMatInstanceDynamic = CreateMaterialInstance(SelectionColor,SelectionOpacity);
	//
	// TArray<FVector> lineVertices;
	// TArray<int32> lineTriangles;
	//
	// 	
	// float lineStart = 0.f;
	// float lineEnd =GridWidth();
	// for (int i = 0; i < NumRows + 1; ++i)
	// {
	// 	lineStart = TileSize * i;
	//
	// 	FVector start = FVector(lineStart,0,0);
	// 	FVector end = FVector(lineStart,lineEnd,0);
	//
	// 	CreateLine(start,end,LineThickness,lineVertices,lineTriangles);
	// }
	//
	// lineEnd =GridHeight();
	// for (int i = 0; i < NumColumns +1; ++i)
	// {
	// 	lineStart = TileSize * i;
	//
	// 	FVector start = FVector(0,lineStart,0);
	// 	FVector end = FVector(lineEnd,lineStart,0);
	// 	CreateLine(start,end,LineThickness,lineVertices,lineTriangles);
	// }
	//
	// // 법선 (Normals), UVs, VertexColors, Tangents 배열을 미리 정의
	// TArray<FVector> Normals;
	// TArray<FVector2D> UV0;
	// TArray<FColor> VertexColors;
	// TArray<FProcMeshTangent> Tangents;
	//
	// // // 기본적으로 모든 정점의 법선을 (0, 0, 1) 방향으로 설정 (위로 향하는 법선)
	// for (int32 i = 0; i < lineVertices.Num(); i++)
	// {
	// 	// Normals.Add(FVector(0.0f, 0.0f, 1.0f)); // 위쪽을 향하는 법선
	// 	// UV0.Add(FVector2D(0.0f, 0.0f));         // 기본 UV 좌표
	// 	VertexColors.Add(FColor::Green);  // 흰색으로 기본 설정
	// 	// Tangents.Add(FProcMeshTangent(1.0f, 0.0f, 0.0f)); // 기본 탱젠트 설정
	// }
	//
	// ProceduralMesh->CreateMeshSection(0,lineVertices,lineTriangles,Normals,UV0,VertexColors,Tangents,true);
	// ProceduralMesh->SetMaterial(0,Cast<UMaterialInstance>(lineMatInstanceDynamic));
	//
	//
	//
	// //selection
	// TArray<FVector> selectionVertices;
	// TArray<int32> selectionTriangles;
	// FVector start = FVector(0,TileSize/2,0);
	// FVector end = FVector(TileSize,TileSize/2,0);
	// CreateLine(start,end,TileSize,selectionVertices,selectionTriangles);
	// SelectionProceduralMesh->CreateMeshSection(0,selectionVertices,selectionTriangles,TArray<FVector>(),TArray<FVector2D>(),TArray<FColor>(),TArray<FProcMeshTangent>(),true);
	// SelectionProceduralMesh->SetMaterial(0,Cast<UMaterialInstance>(selectionMatInstanceDynamic));
}

// Called when the game starts or when spawned
void ABatchGridActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABatchGridActor::CreateLine(FVector start, FVector end, float thickNess, TArray<FVector>& vertices,TArray<int32>& triangles)
{
	float halfThickness = thickNess/2.f;
	FVector direction = end - start;
	direction.Normalize();

	FVector thicknessDirection =  FVector::CrossProduct(direction,FVector(0.0f,0.f,1.0f));


	TArray<int32> triangleArray;
	triangleArray.Add(vertices.Num() + 2);
	triangleArray.Add(vertices.Num() + 1);
	triangleArray.Add(vertices.Num() + 0);
	triangleArray.Add(vertices.Num() + 2);
	triangleArray.Add(vertices.Num() + 3);
	triangleArray.Add(vertices.Num() + 1);
	
	triangles.Append(triangleArray);

	
	TArray<FVector> verticeArray;
	verticeArray.Add(start + (thicknessDirection * halfThickness)); //0
	verticeArray.Add(end + (thicknessDirection * halfThickness)); //1
	verticeArray.Add(start - (thicknessDirection * halfThickness)); //2
	verticeArray.Add(end - (thicknessDirection * halfThickness)); //3
	vertices.Append(verticeArray);

	
}

float ABatchGridActor::GridWidth()
{
	return NumColumns * TileSize;
}

float ABatchGridActor::GridHeight()
{
	return NumRows * TileSize;
}

UMaterialInstanceDynamic* ABatchGridActor::CreateMaterialInstance(FLinearColor color, float opacity)
{
	UMaterialInstanceDynamic* matinstanceDynamic =  ProceduralMesh->CreateDynamicMaterialInstance(0,GridMaterial);
	matinstanceDynamic->SetVectorParameterValue(TEXT("Color"), color);
	matinstanceDynamic->SetScalarParameterValue(TEXT("Opacity"), opacity);

	return matinstanceDynamic; 
}

void ABatchGridActor::SetNumRowsAndColumes(int32 numRows, int32 numColumns)
{
	NumRows = numRows;
	NumColumns = numColumns;
}

void ABatchGridActor::MakeBatchGrid()
{
	UMaterialInstanceDynamic* lineMatInstanceDynamic =  CreateMaterialInstance(LineColor,LineOpacity);
	
	UMaterialInstanceDynamic* selectionMatInstanceDynamic = CreateMaterialInstance(SelectionColor,SelectionOpacity);

	TArray<FVector> lineVertices;
	TArray<int32> lineTriangles;

		
	float lineStart = 0.f;
	float lineEnd =GridWidth();
	for (int i = 0; i < NumRows + 1; ++i)
	{
		lineStart = TileSize * i;

		FVector start = FVector(lineStart,0,0);
		FVector end = FVector(lineStart,lineEnd,0);

		CreateLine(start,end,LineThickness,lineVertices,lineTriangles);
	}

	lineEnd =GridHeight();
	for (int i = 0; i < NumColumns +1; ++i)
	{
		lineStart = TileSize * i;

		FVector start = FVector(0,lineStart,0);
		FVector end = FVector(lineEnd,lineStart,0);
		CreateLine(start,end,LineThickness,lineVertices,lineTriangles);
	}

	// 법선 (Normals), UVs, VertexColors, Tangents 배열을 미리 정의
	TArray<FVector> Normals;
	TArray<FVector2D> UV0;
	TArray<FColor> VertexColors;
	TArray<FProcMeshTangent> Tangents;

	ProceduralMesh->CreateMeshSection(0,lineVertices,lineTriangles,Normals,UV0,VertexColors,Tangents,true);
	ProceduralMesh->SetMaterial(0,Cast<UMaterialInstance>(lineMatInstanceDynamic));



	//selection
	TArray<FVector> selectionVertices;
	TArray<int32> selectionTriangles;
	FVector start = FVector(0,TileSize/2,0);
	FVector end = FVector(TileSize,TileSize/2,0);
	CreateLine(start,end,TileSize,selectionVertices,selectionTriangles);
	SelectionProceduralMesh->CreateMeshSection(0,selectionVertices,selectionTriangles,TArray<FVector>(),TArray<FVector2D>(),TArray<FColor>(),TArray<FProcMeshTangent>(),true);
	SelectionProceduralMesh->SetMaterial(0,Cast<UMaterialInstance>(selectionMatInstanceDynamic));
	SelectionProceduralMesh->SetVisibility(false);
}

bool ABatchGridActor::LocationToTile(FVector location, int32& outRow, int32& outColumn)
{
	float x = location.X;
	float actionLocationX = GetActorLocation().X;
	outRow = ((x - actionLocationX) / GridWidth() ) * NumRows;

	float y = location.Y;
	float actionLocationY = GetActorLocation().Y;
	outColumn = ((y - actionLocationY) / GridHeight() ) * NumColumns;
	
	return TileValid(outRow,outColumn);
}

bool ABatchGridActor::TileToGridLocation(int32 row, int32 column, bool isCenter, FVector2d& outGridLocation)
{

	float tileCenter = isCenter ? TileSize /2.f : 0;
	
	outGridLocation.X = row * TileSize + GetActorLocation().X + tileCenter;
	outGridLocation.Y = column * TileSize + GetActorLocation().Y + tileCenter;
	
	return TileValid(row,column);
}

void ABatchGridActor::SetSelectedTile(int32 row, int32 column)
{
	FVector2d gridPos;
	bool isLocate = TileToGridLocation(row,column,false,gridPos);

	if(isLocate)
	{
		SelectionProceduralMesh->SetVisibility(true);

		FVector pos = FVector(gridPos.X,gridPos.Y, GetActorLocation().Z);
		SelectionProceduralMesh->SetWorldLocation(pos);
	}
	else
	{
		SelectionProceduralMesh->SetVisibility(false);
	}
}

bool ABatchGridActor::TileValid(int32 row, int32 column)
{
	bool validRow = row>= 0 && row<NumRows;
	bool validColumn = column >= 0 && column<NumColumns;
	
	return validRow && validColumn;

}

void ABatchGridActor::SetActiveBatchGrid(bool isActive)
{
	SetActorHiddenInGame(!isActive); //액터를 활성, 비활성

	SetActorEnableCollision(isActive); //충돌을 활성, 비활성
}



