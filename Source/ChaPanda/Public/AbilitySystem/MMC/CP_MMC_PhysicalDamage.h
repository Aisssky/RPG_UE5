// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "CP_MMC_PhysicalDamage.generated.h"

class UCP_AttributeSet;

UCLASS()
class CHAPANDA_API UCP_MMC_PhysicalDamage :public UGameplayModMagnitudeCalculation
{
public:
	GENERATED_BODY()

	UCP_MMC_PhysicalDamage();

	UPROPERTY(EditDefaultsOnly,Category="Cha|Damage")
	TObjectPtr<UCurveTable> DamageCurveTable;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Damage")
	FName CurveRowName;

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
