// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Archer_R.generated.h"

/**
 * 
 */
UCLASS()
class CHAPANDA_API UCP_Archer_R : public UCP_GameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 100.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float DetonateRadius = 800.f;

	void Detonate();
 };
