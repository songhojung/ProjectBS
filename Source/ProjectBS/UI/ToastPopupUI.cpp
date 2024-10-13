// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ToastPopupUI.h"
#include "Components/TextBlock.h"




void UToastPopupUI::ShowPopup(FString text)
{
	Text_Toast->SetText( FText::FromString(text));

	GetWorld()->GetTimerManager().SetTimer(Timer,this,&UToastPopupUI::DestroyPopup,2.f,false);

	// RemoveFromParent()
	
}


void UToastPopupUI::DestroyPopup()
{
	RemoveFromParent();
}