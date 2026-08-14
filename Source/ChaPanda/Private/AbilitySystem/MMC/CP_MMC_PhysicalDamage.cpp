// Copyright Aisssky


#include "AbilitySystem/MMC/CP_MMC_PhysicalDamage.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "GameplayTags/CP_Tags.h"

UCP_MMC_PhysicalDamage::UCP_MMC_PhysicalDamage()
{
	RelevantAttributesToCapture.Add(FGameplayEffectAttributeCaptureDefinition(UCP_AttributeSet::GetAttackAttribute(), EGameplayEffectAttributeCaptureSource::Source, true));
	RelevantAttributesToCapture.Add(FGameplayEffectAttributeCaptureDefinition(UCP_AttributeSet::GetDefenseAttribute(), EGameplayEffectAttributeCaptureSource::Target, true));
	RelevantAttributesToCapture.Add(FGameplayEffectAttributeCaptureDefinition(UCP_AttributeSet::GetCritDamageAttribute(), EGameplayEffectAttributeCaptureSource::Source, true));
	RelevantAttributesToCapture.Add(FGameplayEffectAttributeCaptureDefinition(UCP_AttributeSet::GetCritRateAttribute(), EGameplayEffectAttributeCaptureSource::Source, true));
	RelevantAttributesToCapture.Add(FGameplayEffectAttributeCaptureDefinition(UCP_AttributeSet::GetDamageReductionAttribute(), EGameplayEffectAttributeCaptureSource::Target, true));
}

float UCP_MMC_PhysicalDamage::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
FAggregatorEvaluateParameters EvalParams;
	float BaseDamage = Spec.GetSetByCallerMagnitude(CP_Tags::SetByCaller::Damage);
	bool IsCrit=false;
	float Attack = 0.f;
	GetCapturedAttributeMagnitude(RelevantAttributesToCapture[0],Spec,EvalParams, Attack);
	float Defense = 0.f;
	GetCapturedAttributeMagnitude(RelevantAttributesToCapture[1],Spec,EvalParams, Defense);
	float CritDamage = 0.f;
	GetCapturedAttributeMagnitude(RelevantAttributesToCapture[2],Spec,EvalParams, CritDamage);
	float CritRate = 0.f;
	GetCapturedAttributeMagnitude(RelevantAttributesToCapture[3],Spec,EvalParams, CritRate);
	float DamageReduction = 0.f;
	GetCapturedAttributeMagnitude(RelevantAttributesToCapture[4], Spec, EvalParams, DamageReduction);
	float Level = Spec.GetLevel();

	if (FMath::FRand() <= CritRate) {
		IsCrit = true;
	}
	float Result = BaseDamage * (Attack / (Attack + 50)) * (100 / (100 + Defense))
		*(1-FMath::Clamp(DamageReduction,0,100)/100) 
		* (IsCrit ? CritDamage : 1.0);
	return Result;
}
