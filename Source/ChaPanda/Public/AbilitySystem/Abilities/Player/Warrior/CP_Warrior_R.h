// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Warrior_R.generated.h"

UCLASS()
class CHAPANDA_API UCP_Warrior_R : public UCP_GameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData)override;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Cha|Montage")
	TObjectPtr<UAnimMontage> Montage_R;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SelfBuffEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> AllyBuffEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AuraRadius = 600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AuraTickInterval = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BuffDuration = 8.f;

	UFUNCTION()
	void OnMontageEnd();

	void TickAura();

	FTimerHandle AuraTickHandle;
	FTimerHandle DurationHandle;

};
