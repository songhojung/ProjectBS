// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/UIManager.h"

#include "Blueprint/UserWidget.h"

// Static variable initialization
UUIManager* UUIManager::Instance = nullptr;

UUIManager* UUIManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UUIManager>();
		// Ensure that the manager is not garbage collected by adding it to the root
		Instance->AddToRoot();
	}
	return Instance;
}

void UUIManager::AddUI(FString uiName, APlayerController* playerController,FCompletedAddUIDelegate completedCallback)
{

	///Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/MainWidget.MainWidget'
	if(UUserWidget* ui = GetUI(uiName))
	{
		AddWidgetToViewport(ui,playerController);

		completedCallback.ExecuteIfBound(ui);
	}
	else
	{
		///Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/MainUI.MainUI'
		// 예시 위젯 클래스 (Blueprint에서 설정한 위젯 클래스)
		FString uiNameStr = FString::Printf(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/%s.%s_C'"), *uiName, *uiName);
		TSubclassOf<UUserWidget> createdWidgetClass = LoadClass<UUserWidget>(nullptr, *uiNameStr);

		UUserWidget* createdUI = CreateUI(createdWidgetClass,playerController,true);

		completedCallback.ExecuteIfBound(createdUI);
	}

	// if(uiName.Equals(FString("DisplaySwitchUI")) == false)
	// 	OpenDisplaySwitchUI(playerController);
}

void UUIManager::AddPopupUI(FString uiName, APlayerController* playerController,FCompletedAddUIDelegate completedCallback)
{
	FString uiNameStr = FString::Printf(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/%s.%s_C'"), *uiName, *uiName);
	TSubclassOf<UUserWidget> createdWidgetClass = LoadClass<UUserWidget>(nullptr, *uiNameStr);

	UUserWidget* createdUI = CreateUI(createdWidgetClass,playerController,false);

	completedCallback.ExecuteIfBound(createdUI);
}

UUserWidget* UUIManager::CreateUI(TSubclassOf<UUserWidget> widgetClass, APlayerController* playerController,bool isUI)
{

	UUserWidget* createdWidget = CreateWidget<UUserWidget>(playerController,widgetClass);
	if(createdWidget)
	{
		AddWidgetToViewport(createdWidget,playerController);

		if(isUI)
			UIList.Add(createdWidget);
	}

	return createdWidget;
}

void UUIManager::RemoveUI(UUserWidget* userWidget)
{
	if(userWidget == nullptr)
		return;

	if(UIList.Contains(userWidget))
	{
		userWidget->RemoveFromParent();

		UIList.Remove(userWidget);
	}
	
}


void UUIManager::AddWidgetToViewport(UUserWidget* widget, APlayerController* playerController)
{
	if(widget && playerController)
	{
		widget->AddToViewport();//화면에 위젯추가
	}
}

void UUIManager::OpenDisplaySwitchUI(APlayerController* playerController)
{
	if(playerController!=nullptr)
		AddUI("DisplaySwitchUI",playerController);
}


bool UUIManager::IsContainUI(FString uiName)
{
	for (int i = 0; i < UIList.Num(); ++i)
	{
		UUserWidget* ui = UIList[i];

		// UI의 이름에서 '_C'를 제거
		FString actualUIName;
		FString unusedPart;
			
		if (ui->GetName().Split(TEXT("_"), &actualUIName, &unusedPart))
		{
			if(uiName == actualUIName)
			{
				return true;
			}
		}
	}

	return  false;
}

UUserWidget* UUIManager::GetUI(FString uiName)
{
	for (int i = 0; i < UIList.Num(); ++i)
	{
		UUserWidget* ui = UIList[i];

		if(ui !=nullptr)
		{
			// UI의 이름에서 '_C'를 제거
			FString actualUIName;
			FString unusedPart;
			
			if (ui->GetName().Split(TEXT("_"), &actualUIName, &unusedPart))
			{
				if(uiName  == actualUIName)
				{
					return ui;
				}
			}
		}
	}

	return nullptr;

}


