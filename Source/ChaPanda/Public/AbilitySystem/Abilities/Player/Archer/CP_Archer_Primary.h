// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Archer_Primary.generated.h"

class ACP_Projectile;

UCLASS()
class CHAPANDA_API UCP_Archer_Primary : public UCP_GameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Projectile")
	TSubclassOf<ACP_Projectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Targeting")
	float LockRange{ 1500.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Targeting")
	FName TargetTag{ TEXT("Minion") };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Damage")
	float BaseDamage{ 20.f };

	void FireArrow();
};
