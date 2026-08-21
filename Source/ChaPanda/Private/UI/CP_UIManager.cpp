// Copyright Aisssky


#include "UI/CP_UIManager.h"

void UCP_UIManager::RegisterPanel(FGameplayTag Tag, TSubclassOf<UUserWidget> WidgetClass, int32 Zorder)
{
	Classes.Add(Tag, WidgetClass);
}

void UCP_UIManager::Show(FGameplayTag Tag)
{
	if (!Classes.Contains(Tag))return;

	TObjectPtr<UUserWidget>& Instance = Instances.FindOrAdd(Tag);

	if (!Instance) {
		ULocalPlayer* LP = GetLocalPlayer();
		APlayerController* PC = LP ? LP->GetPlayerController(GetWorld()) : nullptr;
		Instance = CreateWidget<UUserWidget>(PC, Classes[Tag]);
		if (Instance)Instance->AddToViewport();
	}
	if (Instance)Instance->SetVisibility(ESlateVisibility::Visible);
}

void UCP_UIManager::Hide(FGameplayTag Tag)
{
	if (TObjectPtr<UUserWidget>* Inst = Instances.Find(Tag)){
		if (*Inst)(*Inst)->SetVisibility(ESlateVisibility::Collapsed);

	}
}
