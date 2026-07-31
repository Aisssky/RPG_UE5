// Copyright Aisssky


#include "AbilitySystem/Abilities/Enemy/CP_SearchForTarget.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystem/AbilityTasks/CP_WaitGameplayEvent.h"
#include "Character/CP_EnemyCharacter.h"
#include "GameplayTags/CP_Tags.h"
#include "Tasks/AITask_MoveTo.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"


UCP_SearchForTarget::UCP_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCP_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<ACP_EnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!OwningEnemy.IsValid())
	{
		return;
	}
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	if (!OwningAIController.IsValid())
	{
		return;
	}

	StartSearch();

	WaitGameplayEventTask = UCP_WaitGameplayEvent::WaitGameplayEventToActorProxy(GetAvatarActorFromActorInfo(), CP_Tags::Events::Enemy::EndAttack);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &ThisClass::EndAttackEventReceived);
	WaitGameplayEventTask->StartActivation();
}

void UCP_SearchForTarget::StartSearch()
{
	if (bDrawDebugs) GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Start Search"));
	if (!OwningEnemy.IsValid()) return;

	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	SearchDelayTask->OnFinish.AddDynamic(this, &ThisClass::Search);
	SearchDelayTask->Activate();
}


void UCP_SearchForTarget::Search()
{

	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	if (!OwningEnemy.IsValid()) return;
	FClosestActorWithTagResult ClosestActorResult = UCP_BlueprintFunctionLibrary::FindClosestActorWithTag(GetAvatarActorFromActorInfo(), SearchOrigin, ChaTags::Player, OwningEnemy->SearchRange);

	TargetBaseCharacter = Cast<ACP_BaseCharacter>(ClosestActorResult.Actor);

	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}
	if (TargetBaseCharacter->IsAlive())
	{
		MoveToTargetandAttack();
	}
	else
	{
		StartSearch();
	}
}


void UCP_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	if(OwningEnemy.IsValid() && !OwningEnemy->bIsBeingLaunched)
	{
		StartSearch();
	}
}

void UCP_SearchForTarget::MoveToTargetandAttack()
{

	if (!OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;

	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}
	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(
		OwningAIController.Get(),
		FVector(),
		TargetBaseCharacter.Get(),
		OwningEnemy->AcceptanceRadius);

	

	MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
	MoveToLocationOrActorTask->ConditionalPerformMove();
}

void UCP_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* AIController)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}
	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());	

	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	AttackDelayTask->OnFinish.AddDynamic(this, &ThisClass::Attack);
	AttackDelayTask->Activate();
}

void UCP_SearchForTarget::Attack()
{
	const FGameplayTag AttackTag = CP_Tags::CPAbilities::Enemy::Attack;
	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}