// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Death.generated.h"

UCLASS()
class CHAPANDA_API UCP_Death : public UCP_GameplayAbility
{
	GENERATED_BODY()
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Montage")
	TObjectPtr<UAnimMontage> Montage_Death;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Death")
	TSubclassOf<UGameplayEffect> DeathEffect;


	UFUNCTION()
	void RespawnCharacter();

	bool bHasRespawned = false;
};