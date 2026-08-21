// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Archer/CP_Archer_E.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
void UCP_Archer_E::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//冷却检查
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//montage
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_E,
		1.0f,
		NAME_None,
		false,
		1.0f
	);

	MontageTask->OnCompleted.AddDynamic(this, &UCP_Archer_E::OnMontageEnd);
	MontageTask->OnBlendOut.AddDynamic(this, &UCP_Archer_E::OnMontageEnd);
	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, ImpactEventTag, nullptr, false, false);
	WaitEventTask->EventReceived.AddDynamic(this, &UCP_Archer_E::OnImpactEvent);
	WaitEventTask->ReadyForActivation();

}

void UCP_Archer_E::OnImpactEvent(FGameplayEventData Payload)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!Avatar->HasAuthority())return;
	TArray<AActor*> HitTargets = UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(
		Avatar,
		800,
		0.f,
		0.f,
		false
	);


	//遍历目标分发伤害
	for (AActor* Target : HitTargets) {
		float Damage = BaseDamage ;

		FGameplayEventData DamagePayload;
		DamagePayload.Instigator = Avatar;
		UCP_BlueprintFunctionLibrary::SendDamageEventToPlayer(
			Target,
			DamageEffectClass,
			DamagePayload,
			CP_Tags::SetByCaller::Damage,
			Damage,
			FGameplayTag()
		);

		//Debuff
		if (PoisonEffectClass && Target) {
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
			if (TargetASC) {
				FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(
					PoisonEffectClass, 1.f, Ctx
				);
				TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}
}

void UCP_Archer_E::OnMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}

