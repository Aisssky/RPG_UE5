// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Archer/CP_Archer_Q.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCP_Archer_Q::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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
		Montage_Q,
		1.0f,
		NAME_None,
		false,
		1.0f
	);

	MontageTask->OnCompleted.AddDynamic(this, &UCP_Archer_Q::OnMontageEnd);
	MontageTask->OnBlendOut.AddDynamic(this, &UCP_Archer_Q::OnMontageEnd);
	MontageTask->ReadyForActivation();


}



void UCP_Archer_Q::TickRain()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!Avatar)return;
	if (!Avatar->HasAuthority())return;


	TArray<AActor*> HitTargets = UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(
		Avatar,
		RainRadius,
		0.f,
		0.f,
		false
	);

	//遍历目标分发伤害
	for (AActor* Target : HitTargets) {

		bool bMinion = Target->ActorHasTag("Minion");
		float Damage = bMinion ? BaseDamage * 1.5f : BaseDamage * 1.f;

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
	}

	if (--RainTicksRemaining <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(RainTimerHandle);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}


void UCP_Archer_Q::OnMontageEnd()
{
	RainTicksRemaining = FMath::CeilToInt(RainDuration);
	GetWorld()->GetTimerManager().SetTimer(
		RainTimerHandle, this, &UCP_Archer_Q::TickRain, 1.f, true, 0.f
	);
}
