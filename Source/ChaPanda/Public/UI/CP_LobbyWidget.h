// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "CP_LobbyWidget.generated.h"

class UHorizontalBox;
class UButton;
class UWidgetSwitcher;
class APlayerState;

UCLASS()
class CHAPANDA_API UCP_LobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> PlayerList;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> PhaseSwitcher;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> StartGameButton;

	UPROPERTY(EditDefaultsOnly,Category="Cha|Lobby")
	TSubclassOf<UUserWidget> PlayerEntryClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|CharacterSelect")
	TSubclassOf<UUserWidget> CharacterSelectWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> CharacterSelectWidgetInstance;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnPhaseChnaged(FGameplayTag OldPhase, FGameplayTag NewPhase);

	UFUNCTION()
	void RefreshPlayerList();

	UFUNCTION()
	void OnStartGameClicked();

	UFUNCTION()
	void OnHostChangedHandler(APlayerState* HostPlayerState);

	UFUNCTION()
	void OnPlayerListChangedHandler();

	void RefreshStartButtonState();
};
