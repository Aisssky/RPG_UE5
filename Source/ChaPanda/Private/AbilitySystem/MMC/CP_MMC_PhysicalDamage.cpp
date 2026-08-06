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

	float Level = Spec.GetLevel();

	if (FMath::FRand() <= CritRate) {
		IsCrit = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("[MMC] Attack=%.1fDefense = % .1f CritRate = % .3f CritDamage = % .1f"),Attack, Defense, CritRate, CritDamage);
	UE_LOG(LogTemp, Warning, TEXT("[MMC] SourceASC=%s |TargetASC = % s"),* GetNameSafe(Spec.GetContext().GetInstigatorAbilitySystemComponent()),*GetNameSafe(Spec.GetContext().GetEffectCauser()));
	float Result = BaseDamage * (Attack / (Attack + 50)) * (100 / (100 + Defense))*(IsCrit?CritDamage:1.0);
	UE_LOG(LogTemp, Warning, TEXT("[MMC] Result=%.2f"), Result);
	return Result;
}
