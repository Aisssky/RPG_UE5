// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "CP_UIManager.generated.h"

UCLASS()
class CHAPANDA_API UCP_UIManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
public:

	void RegisterPanel(FGameplayTag Tag, TSubclassOf<UUserWidget> WidgetClass, int32 Zorder);
	void Show(FGameplayTag Tag);
	void Hide(FGameplayTag Tag);

private:
	TMap<FGameplayTag, TSubclassOf<UUserWidget>> Classes;
	TMap<FGameplayTag, TObjectPtr<UUserWidget>> Instances;
};
