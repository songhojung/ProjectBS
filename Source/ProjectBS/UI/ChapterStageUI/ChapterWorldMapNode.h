// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "ChapterWorldMapNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UChapterWorldMapNode : public UUserWidget
{
	GENERATED_BODY()



public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node)
	int NodeId;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Node)
	TArray<UImage*> Fogs;
};
