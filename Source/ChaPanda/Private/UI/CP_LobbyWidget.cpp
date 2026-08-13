// Copyright Aisssky


#include "UI/CP_LobbyWidget.h"
#include "UI/CP_PlayerEntry.h"
#include "Core/CP_GameState.h"
#include "Core/CP_LobbyGameMode.h"
#include "Player/CP_PlayerController.h"
#include "Player/CP_PlayerState.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"
#include "GameplayTags/CP_Tags.h"


void UCP_LobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>())
	{
		GS->OnGamePhaseChanged.AddDynamic(this, &UCP_LobbyWidget::OnPhaseChnaged);
	}

	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &UCP_LobbyWidget::OnStartGameClicked);
		StartGameButton->SetIsEnabled(false);
	}

	RefreshPlayerList();
}

void UCP_LobbyWidget::NativeDestruct()
{
	if (ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>()) {
		GS->OnGamePhaseChanged.RemoveDynamic(this,&UCP_LobbyWidget::OnPhaseChnaged);
	}

	Super::NativeDestruct();
}

void UCP_LobbyWidget::OnPhaseChnaged(FGameplayTag OldPhase, FGameplayTag NewPhase)
{
	if (!PhaseSwitcher)return;

	if (NewPhase == CP_Tags::GamePhase::Lobby) {
		PhaseSwitcher->SetActiveWidgetIndex(0);
	}
	else if (NewPhase == CP_Tags::GamePhase::CharacterSelect) {
		PhaseSwitcher->SetActiveWidgetIndex(1);
	}

	RefreshPlayerList();

}

void UCP_LobbyWidget::RefreshPlayerList()
{
	if (!PlayerList || !PlayerEntryClass)return;

	PlayerList->ClearChildren();
	ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>();
	if (!IsValid(GS))return;

	for (APlayerState* PS : GS->PlayerArray) {
		ACP_PlayerState* CPS = Cast<ACP_PlayerState>(PS);
		if (!IsValid(CPS))continue;

		UUserWidget* EntryWidget = CreateWidget<UUserWidget>(this, PlayerEntryClass);
		UCP_PlayerEntry* Entry = Cast<UCP_PlayerEntry>(EntryWidget);
		if (Entry) {
			Entry->SetPlayerName(CPS->GetPlayerName());
		}

		PlayerList->AddChildToHorizontalBox(EntryWidget);
	}
}

void UCP_LobbyWidget::OnStartGameClicked()
{
	ACP_PlayerController* PC = Cast<ACP_PlayerController>(GetOwningPlayer());
	if (IsValid(PC)) {
		PC->Server_StartGame(TEXT("CPMap"));
	}
}
