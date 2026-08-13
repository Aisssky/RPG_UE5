// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "CP_HeroCard.generated.h"

class UBorder;
class UTextBlock;
class UButton;

UCLASS()
class CHAPANDA_API UCP_HeroCard : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetupCard(FGameplayTag InHeroTag, const FText& InHeroName);

	void SetCardState(bool bInIsTaken, bool bInIsMyPick);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> CardBorder;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> HeroNameText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> LockButton;

private:
	FGameplayTag HeroTag;
	bool bIsTaken=false;
	bool bIsMyPick=false;

	void RefreshCardVisual();

	UFUNCTION()
	void OnSelectClicked();

	UFUNCTION()
	void OnLockClicked();

};
