// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "CP_WaitGameplayEvent.generated.h"

UCLASS()
class CHAPANDA_API UCP_WaitGameplayEvent : public UAbilityAsync_WaitGameplayEvent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable,Category="Ability|Async",meta=(DefaultToSelf="TargetActor",BlueprintInternalUseOnly="TRUE"))
	static UCP_WaitGameplayEvent * WaitGameplayEventToActorProxy(AActor* TargetActor,FGameplayTag EventTag,bool OnlyTriggerOnce = false,bool OnlyMatchExact = true);

	void StartActivation();
};
