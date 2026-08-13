// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "CP_GameState.generated.h"

//UI委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnGamePhaseChanged, FGameplayTag, OldPhase, FGameplayTag, NewPhase);
UCLASS()
class CHAPANDA_API ACP_GameState : public AGameState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	//游戏阶段
	UPROPERTY(ReplicatedUsing=OnRep_GamePhaseTag)
	FGameplayTag GamePhaseTag;

	UFUNCTION()
	void OnRep_GamePhaseTag(FGameplayTag OldPhase);


	UPROPERTY(BlueprintAssignable,Category="Cha|GamePhase")
	FOnGamePhaseChanged OnGamePhaseChanged;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|GamePhase", meta = (AllowPrivateAccess = "true"))
	float PhaseStartTime = 0.0f;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|GamePhase", meta = (AllowPrivateAccess = "true"))
	float PhaseDuration = -1.0f;

	void SetGamePhase(FGameplayTag NewPhase, float Duration = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "Cha|GamePhase")
	float GetRemainingPhaseTime() const;

	UFUNCTION(BlueprintPure, Category = "Cha|GamePhase")
	bool IsInPhase(FGameplayTag Tag) const { return GamePhaseTag == Tag; };

	UPROPERTY(Replicated,BlueprintReadOnly,Category="Cha|Match",meta=(AllowPrivateAccess="true"))
	float MatchTimeElapsed = 0.f;

};
