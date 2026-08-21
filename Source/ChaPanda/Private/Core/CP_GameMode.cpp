// Copyright Aisssky


#include "Core/CP_GameMode.h"
#include "Character/CP_PlayerCharacter.h"
#include "Player/CP_PlayerController.h"
#include "Core/CP_GameState.h"
#include "Player/CP_PlayerState.h"
#include "Core/CP_LobbyGameMode.h"
#include "Data/CP_HeroDatalog.h"
#include "Data/CP_HeroData.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Core/CP_GameInstance.h"
#include "GameplayTags/CP_Tags.h"

ACP_GameMode::ACP_GameMode()
{
	PlayerControllerClass = ACP_PlayerController::StaticClass();
	PlayerStateClass = ACP_PlayerState::StaticClass();
	GameStateClass = ACP_GameState::StaticClass();
	DefaultPawnClass = ACP_PlayerCharacter::StaticClass();

	//bDelayedStart = true;
}

void ACP_GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ACP_GameState* GS = GetGameState<ACP_GameState>())
	{
		GS->SetGamePhase(CP_Tags::GamePhase::InProgress);
	}

	GetWorldTimerManager().SetTimer(WaveTimerHandle, this, &ACP_GameMode::StartNextWave, 3.f, false);
}

void ACP_GameMode::StartNextWave()
{
	if (!HasAuthority()) return;
	ACP_GameState* GS = GetGameState<ACP_GameState>();
	if (!GS) return;

	GS->CurrentWave++;
	bWaveInProgress = true;
	SpawnWave(GS->CurrentWave);

}

void ACP_GameMode::SpawnWave(int32 Wave)
{
	ACP_GameState* GS = GetGameState<ACP_GameState>();
	if (!GS)return;

	int32 MeleeCount = 0, RangedCount = 0;

	if (Wave <= 2) {
		MeleeCount = 2 + Wave * 2;
	}
	else if (Wave <= 4) {
		MeleeCount = 4; RangedCount = 2;
	}
	else if (Wave <= 6) {
		MeleeCount = 6; RangedCount = 4;
	}
	else {
		MeleeCount = 8; RangedCount = 6;
	}
	TArray<FVector>SpawnLocs;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It) {
		SpawnLocs.Add(It->GetActorLocation());
	}
	if (SpawnLocs.Num() == 0) SpawnLocs.Add(FVector::ZeroVector);

	// Spawn 近战 + 远程
	auto Spawn = [&](TSubclassOf<ACP_EnemyCharacter> Class, int32 Count) {
		for (int32 i = 0; i < Count; ++i)
		{
			FVector Loc = SpawnLocs[FMath::RandRange(0, SpawnLocs.Num() - 1)]
				+ FVector(FMath::RandRange(-300.f, 300.f), FMath::RandRange(-300.f, 300.f), 0.f);
			FActorSpawnParameters P;
			P.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			if (GetWorld()->SpawnActor<ACP_EnemyCharacter>(Class, Loc, FRotator::ZeroRotator, P))
				GS->AliveEnemyCount++;
		}
		};

	if (MeleeEnemyClass)  Spawn(MeleeEnemyClass, MeleeCount);
	if (RangedEnemyClass)  Spawn(RangedEnemyClass, RangedCount);

}

void ACP_GameMode::OnEnemyKilled()
{
	if (!HasAuthority()) return;
	ACP_GameState* GS = GetGameState<ACP_GameState>();
	if (!GS) return;
	if (!bWaveInProgress || GS->AliveEnemyCount > 0) return;

	// 清完一波延迟刷下一波
	bWaveInProgress = false;
	GetWorldTimerManager().SetTimer(WaveTimerHandle, this,
		&ACP_GameMode::StartNextWave, WaveInterval, false);
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
	ACP_PlayerState* PS = InController ? InController->GetPlayerState<ACP_PlayerState>():nullptr;

	if (PS && HeroDatalog) {
		FGameplayTag Tag = PS ? PS->GetSelectedHeroTag() : FGameplayTag();

		for (UCP_HeroData* HeroData : HeroDatalog->Heroes) {
			if (!HeroData)continue;
			if (HeroData->HeroTag == Tag)return HeroData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

