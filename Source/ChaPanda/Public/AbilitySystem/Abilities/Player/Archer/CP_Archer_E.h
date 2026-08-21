// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Archer_E.generated.h"

/**
 * 
 */
UCLASS()
class CHAPANDA_API UCP_Archer_E : public UCP_GameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Montage")
	TObjectPtr<UAnimMontage> Montage_E;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> PoisonEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ImpactEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 50.f;

	UFUNCTION()
	void OnImpactEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageEnd();
};
