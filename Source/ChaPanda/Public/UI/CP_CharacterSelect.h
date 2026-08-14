// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Player/CP_PlayerState.h"
#include "Data/CP_HeroCatalog.h"
#include "CP_CharacterSelect.generated.h"

class UTextBlock;
class UCP_HeroCard;
class UWrapBox;
UCLASS()
class CHAPANDA_API UCP_CharacterSelect : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	TSubclassOf<UCP_HeroCard> HeroCardClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> CardContainer; 

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StatusText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedText;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UCP_HeroCatalog> HeroCatalog;

private:

	UPROPERTY()
	TArray<TObjectPtr<UCP_HeroCard>> CardWidgets;
	void RefreshAllCards();

	UFUNCTION()
	void OnHeroSelectionChangedHandler(FGameplayTag OldTag, FGameplayTag NewTag);

	UFUNCTION()
	void OnHeroLockChangedHandler(bool bLocked);
};
