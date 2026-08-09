// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Warrior/CP_Warrior_Q.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCP_Warrior_Q::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//冷却检查
	if(!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	//montage
	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_Q,
		1.0f,
		NAME_None,
		false,
		1.0f
		);

	MontageTask->OnCompleted.AddDynamic(this, &UCP_Warrior_Q::OnMontageEnd);
	MontageTask->OnBlendOut.AddDynamic(this, &UCP_Warrior_Q::OnMontageEnd);
	MontageTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		ImpactEventTag,
		nullptr,
		false,
		false
	);
	WaitEventTask->EventReceived.AddDynamic(this, &UCP_Warrior_Q::OnImpactEvent);
	WaitEventTask->ReadyForActivation();

}

void UCP_Warrior_Q::OnImpactEvent(FGameplayEventData Payload)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector Center = Avatar->GetActorLocation();

	TArray<AActor*> HitTargets = UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(
		Avatar,
		OuterR,
		0.f,
		0.f,
		false
	);

	UCP_BlueprintFunctionLibrary::ApplyKnockback(
		Avatar, HitTargets,
		InnerR, OuterR,
		InnerForce,
		45.f,
		false
	);

	//遍历目标分发伤害
	for (AActor* Target : HitTargets) {
		float Dist = FVector::Dist(Center, Target->GetActorLocation());
		bool bInner = Dist <= InnerR;


		float Damage = bInner?BaseDamage*2.0f:BaseDamage*0.5f;

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
		TSubclassOf<UGameplayEffect> DebuffClass = bInner ? StunEffectClass : SlowEffectClass;
		if (DebuffClass && Target) {
			UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
			if (TargetASC) {
				FGameplayEffectContextHandle Ctx = TargetASC->MakeEffectContext();
				FGameplayEffectSpecHandle Spec = TargetASC->MakeOutgoingSpec(
					DebuffClass,1.f,Ctx
				);
				TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
			}
		}
	}
}

void UCP_Warrior_Q::OnMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

}
