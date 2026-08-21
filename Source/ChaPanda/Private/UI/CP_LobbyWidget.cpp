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
		GS->OnHostChanged.AddDynamic(this, &UCP_LobbyWidget::OnHostChangedHandler);
		GS->OnPlayerListChanged.AddDynamic(this, &UCP_LobbyWidget::OnPlayerListChangedHandler);
	}

	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &UCP_LobbyWidget::OnStartGameClicked);
	}

	RefreshStartButtonState();
	RefreshPlayerList();
}

void UCP_LobbyWidget::NativeDestruct()
{
	if (CharacterSelectWidgetInstance)
	{
		CharacterSelectWidgetInstance->RemoveFromParent();
		CharacterSelectWidgetInstance = nullptr;
	}

	if (ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>()) {
		GS->OnGamePhaseChanged.RemoveDynamic(this,&UCP_LobbyWidget::OnPhaseChnaged);
		GS->OnHostChanged.RemoveDynamic(this,&UCP_LobbyWidget::OnHostChangedHandler);
		GS->OnPlayerListChanged.RemoveDynamic(this,&UCP_LobbyWidget::OnPlayerListChangedHandler);
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
		if (!CharacterSelectWidgetInstance && CharacterSelectWidgetClass)
		{
			CharacterSelectWidgetInstance = CreateWidget<UUserWidget>(this, CharacterSelectWidgetClass);
			CharacterSelectWidgetInstance->AddToViewport();
		}
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

void UCP_LobbyWidget::OnHostChangedHandler(APlayerState* HostPlayerState)
{
	RefreshStartButtonState();
}

void UCP_LobbyWidget::OnPlayerListChangedHandler()
{
	RefreshPlayerList();
}

void UCP_LobbyWidget::RefreshStartButtonState()
{
	if (!StartGameButton) return;

	bool bIsHost = false;
	APlayerController* PC = GetOwningPlayer();
	ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>();

	if (PC)
	{
		if (PC->HasAuthority())
		{
			bIsHost = true;
		}
		else if (GS)
		{
			bIsHost = (GS->HostPlayerState != nullptr && GS->HostPlayerState == PC->PlayerState);
		}

		FString MyPSName = TEXT("NULL");
		if (PC->PlayerState) { MyPSName = PC->PlayerState->GetPlayerName(); }
		FString HostPSName = TEXT("NULL");
		if (GS && GS->HostPlayerState) { HostPSName = GS->HostPlayerState->GetPlayerName(); }

		UE_LOG(LogTemp, Warning, TEXT("[HostDebug] HasAuthority=%d bIsHost=%d MyPS=%s HostPS=%s"),
			(int32)PC->HasAuthority(), (int32)bIsHost, *MyPSName, *HostPSName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[HostDebug] GetOwningPlayer()==NULL"));
	}

	StartGameButton->SetIsEnabled(bIsHost);
}
