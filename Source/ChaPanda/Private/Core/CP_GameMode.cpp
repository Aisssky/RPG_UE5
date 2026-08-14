// Copyright Aisssky


#include "Core/CP_GameMode.h"
#include "Character/CP_PlayerCharacter.h"
#include "Player/CP_PlayerController.h"
#include "Core/CP_GameState.h"
#include "Player/CP_PlayerState.h"
#include "Core/CP_LobbyGameMode.h"

ACP_GameMode::ACP_GameMode()
{
	PlayerControllerClass = ACP_PlayerController::StaticClass();
	PlayerStateClass = ACP_PlayerState::StaticClass();
	GameStateClass = ACP_GameState::StaticClass();
	DefaultPawnClass = ACP_PlayerCharacter::StaticClass();

	//bDelayedStart = true;
}

void ACP_GameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp,Warning,TEXT("[GameMode] InitGame | Map=%s | Options=%s"), *MapName, *Options);

}

void ACP_GameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("[GameMode] PreLogin | Address=%s"), *Address);
}

void ACP_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] PostLogin | Player=%s | NumPlayers=%d"),
		*NewPlayer->GetName(), GetNumPlayers());
}

void ACP_GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	UE_LOG(LogTemp, Warning, TEXT("[GameMode] Logout | Player=%s | NumPlayers=%d"),
		*Exiting->GetName(), GetNumPlayers());
}




UClass* ACP_GameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	const ACP_PlayerController* PC=Cast<ACP_PlayerController>(InController);
	if(!IsValid(PC)) {
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}

	const FGameplayTag SelectedHeroTag = PC->GetSelectedHeroTag();
	if(!SelectedHeroTag.IsValid()) {
		return Super::GetDefaultPawnClassForController_Implementation(InController);
	}

	if(const TSubclassOf<APawn>* FoundClass = HeroPawnClasses.Find(SelectedHeroTag)) {
		return *FoundClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

