// Copyright Aisssky


#include "Core/CP_GameState.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"

void ACP_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACP_GameState, GamePhaseTag);
	DOREPLIFETIME(ACP_GameState, PhaseStartTime);
	DOREPLIFETIME(ACP_GameState, PhaseDuration);
	DOREPLIFETIME(ACP_GameState, MatchTimeElapsed);
}

void ACP_GameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (GetMatchState() == MatchState::InProgress) {
		MatchTimeElapsed += DeltaSeconds;
	}
}

void ACP_GameState::BeginPlay()
{
	Super::BeginPlay();

	//只有服务器开tick
	SetActorTickEnabled(HasAuthority());
}

void ACP_GameState::OnRep_GamePhaseTag(FGameplayTag OldPhase)
{
	//广播UI切换
	OnGamePhaseChanged.Broadcast(OldPhase, GamePhaseTag);
}

void ACP_GameState::SetGamePhase(FGameplayTag NewPhase, float Duration)
{
	if (!HasAuthority())return;
	FGameplayTag OldPhase = GamePhaseTag;
	GamePhaseTag = NewPhase;
	PhaseStartTime = GetServerWorldTimeSeconds();
	PhaseDuration = Duration;


	OnGamePhaseChanged.Broadcast(OldPhase, NewPhase);
}

float ACP_GameState::GetRemainingPhaseTime() const
{
	if (PhaseDuration < 0.f) {
		return -1.f;
	}
	float Elapsed = GetServerWorldTimeSeconds() - PhaseStartTime;
	return FMath::Max(0.f,PhaseDuration-Elapsed);
}
