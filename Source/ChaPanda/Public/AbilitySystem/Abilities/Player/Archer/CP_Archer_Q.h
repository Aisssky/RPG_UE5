// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "CP_Archer_Q.generated.h"

/**
 * 
 */
UCLASS()
class CHAPANDA_API UCP_Archer_Q : public UCP_GameplayAbility
{
	GENERATED_BODY()
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;
protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Cha|Montage")
	TObjectPtr<UAnimMontage> Montage_Q;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Effect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;


	FTimerHandle RainTimerHandle;
	int32 RainTicksRemaining = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Q")
	float BaseDamage = 30.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Q")
	float RainRadius = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Q")
	float RainDuration = 3.f;

	UFUNCTION()
	void TickRain();

	UFUNCTION()
	void OnMontageEnd();
};
