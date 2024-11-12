// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIManager.generated.h"

DECLARE_DELEGATE_OneParam(FCompletedAddUIDelegate, UUserWidget*);


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

	void AddUI(FString uiName,APlayerController* playerController, FCompletedAddUIDelegate completedCallback = nullptr);
	void AddPopupUI(FString uiName,APlayerController* playerController, FCompletedAddUIDelegate completedCallback = nullptr);

	UUserWidget* CreateUI(TSubclassOf<UUserWidget> widgetClass,APlayerController* playerController,bool isUI);

	void RemoveUI(UUserWidget* userWidget);

protected:
	void AddWidgetToViewport(UUserWidget* widget,APlayerController* playerController );
	void OpenDisplaySwitchUI(APlayerController* playerController);

public:
	bool IsContainUI(FString uiName);
	UUserWidget* GetUI(FString uiName);



	
};
