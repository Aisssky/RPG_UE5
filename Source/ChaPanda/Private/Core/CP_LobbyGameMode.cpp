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

	bDelayedStart = true;
}

void ACP_LobbyGameMode::StartGame(const FString& MapName)
{
	if (!CanStartGame()) {
		UE_LOG(LogTemp, Warning, TEXT("[Lobby] Cannot start: not all players locked"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Lobby] ServerTravel → %s"), *MapName);

	GetWorld()->ServerTravel(MapName + TEXT("?listen"));

	//servertravel
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


