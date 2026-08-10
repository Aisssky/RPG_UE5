// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Warrior_E.generated.h"

UCLASS()
class CHAPANDA_API UCP_Warrior_E : public UCP_GameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage_E_Tap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage_E_Charge;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage_E_Full;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ImpactEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TapRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FullRadius = 500.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ChargeTime = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TapForce = 300.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float FullLaunchForce = 600.f;

	UFUNCTION()
	void OnImpactEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageEnd();

	UFUNCTION()
	void OnInputRelease(float TimeHeld);

private:
	bool bChargeComplete = false;
	bool bSweepExecuted = false;

};
