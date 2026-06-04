// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include  "CP_SearchForTarget.generated.h"

namespace EPathFollowingResult
{
	enum Type :int;
}

class UAITask_MoveTo;
class ACP_BaseCharacter;
class UAbilityTask_WaitDelay;
class ACP_EnemyCharacter;
class AAIController;
class UCP_WaitGameplayEvent;


UCLASS()
class CHAPANDA_API UCP_SearchForTarget : public UCP_GameplayAbility
{
	GENERATED_BODY()
public:

	UCP_SearchForTarget();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TWeakObjectPtr<ACP_EnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;
	TWeakObjectPtr<ACP_BaseCharacter> TargetBaseCharacter;

private:

	UPROPERTY()
	TObjectPtr<UCP_WaitGameplayEvent> WaitGameplayEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> SearchDelayTask;

	UPROPERTY()
	TObjectPtr<UAITask_MoveTo> MoveToLocationOrActorTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitDelay> AttackDelayTask;

	void StartSearch();

	UFUNCTION()
	void EndAttackEventReceived(FGameplayEventData Payload);

	UFUNCTION()
	void Search();

	void MoveToTargetandAttack();

	UFUNCTION()
	void AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result,AAIController* AIController);

	UFUNCTION()
	void Attack();



};
