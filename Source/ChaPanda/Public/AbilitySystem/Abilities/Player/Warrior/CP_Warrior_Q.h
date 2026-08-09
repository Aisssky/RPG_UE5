// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Warrior_Q.generated.h"

UCLASS()
class CHAPANDA_API UCP_Warrior_Q : public UCP_GameplayAbility
{
	GENERATED_BODY()
	
public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
		const FGameplayEventData* TriggerEventData) override;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> StunEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> SlowEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ImpactEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BaseDamage = 50.f;       // SetByCaller 基础伤害

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float InnerForce = 200;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float OuterForce = 200;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float InnerR = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float OuterR = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Montage")
	TObjectPtr<UAnimMontage> Montage_Q;

	UFUNCTION()
	void OnImpactEvent(FGameplayEventData Payload);

	UFUNCTION()
	void OnMontageEnd();

};
