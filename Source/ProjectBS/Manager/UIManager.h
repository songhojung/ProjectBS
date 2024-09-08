// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBS_API UUIManager : public UObject
{
	GENERATED_BODY()

	// Singleton instance
	static UUIManager* Instance;

public:
	// Static instance getter
	static UUIManager* Get();
	
public:
	UPROPERTY(Transient)
	TArray<UUserWidget*> UIList;


	
public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void AddUI(FString uiName,APlayerController* playerController);
	

	UFUNCTION(BlueprintCallable, Category = "UI")
	UUserWidget* CreateUI(TSubclassOf<UUserWidget> widgetClass,APlayerController* playerController);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void RemoveUI(UUserWidget* userWidget);

protected:
	void AddWidgetToViewport(UUserWidget* widget,APlayerController* playerController );

public:
	bool IsContainUI(FString uiName);
	UUserWidget* GetUI(FString uiName);



	
};
