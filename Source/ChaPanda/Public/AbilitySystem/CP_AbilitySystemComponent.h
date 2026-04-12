// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "CP_AbilitySystemComponent.generated.h"

UCLASS(ClassGroup =(Custom), meta =(BlueprintSpawnableComponent))
class CHAPANDA_API UCP_AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRep_ActivateAbilities() override;

private:

	void HandleActivateAbility(const FGameplayAbilitySpec& AbilitySpec);
};
