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

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, 
		bool bReplicateEndAbility, bool bWasCancelled) override;

protected:

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

};
