// Copyright Aisssky


#include "AbilitySystem/AbilityTasks/CP_WaitGameplayEvent.h"

UCP_WaitGameplayEvent* UCP_WaitGameplayEvent::WaitGameplayEventToActorProxy(AActor* TargetActor, FGameplayTag EventTag, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UCP_WaitGameplayEvent* MyObj = NewObject<UCP_WaitGameplayEvent>();
	MyObj->SetAbilityActor(TargetActor);
	MyObj->Tag = EventTag;
	MyObj->OnlyTriggerOnce = OnlyTriggerOnce;
	MyObj->OnlyMatchExact = OnlyMatchExact;
	return MyObj;
}

void UCP_WaitGameplayEvent::StartActivation()
{
	Activate();
}
