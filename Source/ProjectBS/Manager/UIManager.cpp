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

void UUIManager::AddUI(FString uiName, APlayerController* playerController)
{

	///Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/MainWidget.MainWidget'
	if(UUserWidget* ui = GetUI(uiName))
	{
		AddWidgetToViewport(ui,playerController);
	}
	else
	{
		///Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/MainUI.MainUI'
		// 예시 위젯 클래스 (Blueprint에서 설정한 위젯 클래스)
		FString uiNameStr = FString::Printf(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/ProjectBS/UI/%s.%s_C'"), *uiName, *uiName);
		TSubclassOf<UUserWidget> createdWidgetClass = LoadClass<UUserWidget>(nullptr, *uiNameStr);

		CreateUI(createdWidgetClass,playerController);
	}
	
}

UUserWidget* UUIManager::CreateUI(TSubclassOf<UUserWidget> widgetClass, APlayerController* playerController)
{

	UUserWidget* createdWidget = CreateWidget<UUserWidget>(playerController,widgetClass);
	if(createdWidget)
	{
		AddWidgetToViewport(createdWidget,playerController);

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

bool UUIManager::IsContainUI(FString uiName)
{
	for (int i = 0; i < UIList.Num(); ++i)
	{
		UUserWidget* ui = UIList[i];
		
		if(uiName == ui->GetName())
		{
			return true;
		}
	}

	return  false;
}

UUserWidget* UUIManager::GetUI(FString uiName)
{
	for (int i = 0; i < UIList.Num(); ++i)
	{
		UUserWidget* ui = UIList[i];
		
		if(ui !=nullptr && uiName  == ui->GetName())
		{
			return ui;
		}
	}

	return nullptr;

}

