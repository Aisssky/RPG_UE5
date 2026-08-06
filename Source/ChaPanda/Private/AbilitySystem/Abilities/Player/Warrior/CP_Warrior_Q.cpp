// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Warrior/CP_Warrior_Q.h"

void UCP_Warrior_Q::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask::WaitGameplayEvent::WaitGameplayEvent(this,ImpactEventTag);

	Task->EventReceived.AddDynamic(this, &UCP_Warrior_Q::OnImpactEventReceived);
	Task->ReadyForActivation();
}
