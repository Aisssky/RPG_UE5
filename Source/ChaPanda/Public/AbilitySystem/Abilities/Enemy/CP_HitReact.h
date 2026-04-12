// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_HitReact.generated.h"

UCLASS()
class CHAPANDA_API UCP_HitReact : public UCP_GameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Cha|Abilities")
	void CachedHitDirectionVectors(AActor* Instigator);

	UPROPERTY(BlueprintReadOnly, Category = "Cha|Abilities")
	FVector AvatorForward;

	UPROPERTY(BlueprintReadOnly, Category = "Cha|Abilities")
	FVector ToInstigator;

};
