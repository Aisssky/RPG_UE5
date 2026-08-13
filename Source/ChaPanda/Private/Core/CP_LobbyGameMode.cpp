// Copyright Aisssky


#include "Core/CP_LobbyGameMode.h"
#include "Core/CP_GameState.h"
#include "Player/CP_PlayerState.h"
#include "GameplayTags/CP_Tags.h"
#include "GameFramework/PlayerController.h"
#include "Player/CP_PlayerController.h"

ACP_LobbyGameMode::ACP_LobbyGameMode()
{
	PlayerControllerClass = ACP_PlayerController::StaticClass();
	PlayerStateClass = ACP_PlayerState::StaticClass();
	GameStateClass = ACP_GameState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	bDelayedStart = true;
}

void ACP_LobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ACP_GameState* GS = GetGameState<ACP_GameState>();
	if (!GS)return;

	if (GS->GamePhaseTag != CP_Tags::GamePhase::CharacterSelect)return;

	bool bCountdownOver = GS->PhaseDuration >= 0.f && GS->GetRemainingPhaseTime() <= 0.f;

	if ((bCountdownOver || CanStartGame()) && !bMatchTravelPending) {
		bMatchTravelPending = true;
		GetWorld()->ServerTravel(PendingMapName + TEXT("?listen"));
	}
	
}

void ACP_LobbyGameMode::StartGame(const FString& MapName)
{
	PendingMapName = MapName;
	bMatchTravelPending = false;

	if (ACP_GameState* GS = GetGameState<ACP_GameState>()) {
		GS->SetGamePhase(CP_Tags::GamePhase::CharacterSelect, CharacterSelectDuration);
	}
}

int32 ACP_LobbyGameMode::GetLockedPlayerCount()
{
	int32 Count = 0;
	for (APlayerState* PS : GameState->PlayerArray) {
		const ACP_PlayerState* CPS = Cast<ACP_PlayerState>(PS);
		if (CPS && CPS->IsHeroLocked()) { Count++; }
	}
	return Count;
}

bool ACP_LobbyGameMode::CanStartGame()
{
	return  GetLockedPlayerCount() > 0 && GetLockedPlayerCount() == GetNumPlayers();
}


bool ACP_LobbyGameMode::ReadyToStartMatch_Implementation()
{
	return false;
}

void ACP_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//游戏大厅 还是没有放模型的打算
	if (ACP_GameState* GS = GetGameState<ACP_GameState>()) {
		if (GS->GamePhaseTag != CP_Tags::GamePhase::Lobby) {
			GS->SetGamePhase(CP_Tags::GamePhase::Lobby);
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("[Lobby] Player %s joined (%d/%d)"),*NewPlayer->GetName(), GetNumPlayers(), GetMaxPlayers());
}


UClass* ACP_LobbyGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	return nullptr;
}


