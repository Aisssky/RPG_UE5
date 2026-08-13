// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Player/CP_PlayerState.h"
#include "CP_CharacterSelect.generated.h"

class UTextBlock;
class UCP_HeroCard;

UCLASS()
class CHAPANDA_API UCP_CharacterSelect : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCP_HeroCard> CardWarrior;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCP_HeroCard> CardMage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCP_HeroCard> CardDoctor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCP_HeroCard> CardArcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCP_HeroCard> CardAssassin;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedText;

private:
	void RefreshAllCards();

	UFUNCTION()
	void OnHeroSelectionChangedHandler(FGameplayTag OldTag, FGameplayTag NewTag);

	UFUNCTION()
	void OnHeroLockChangedHandler(bool bLocked);
};
