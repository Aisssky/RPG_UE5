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

	UFUNCTION(BlueprintCallable,Category = "Cha|Abilities")
	void SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level);

	UFUNCTION(BlueprintCallable, Category = "Cha|Abilities")
	void AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 1);


private:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	void HandleActivateAbility(const FGameplayAbilitySpec& AbilitySpec);
};
