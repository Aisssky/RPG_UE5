// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Warrior/CP_Warrior_E.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCP_Warrior_E::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// 冷却检查
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	bChargeComplete = false;
	bSweepExecuted = false;

	//蓄力
	UAbilityTask_PlayMontageAndWait* ChargeMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_E_Charge,
		1.0f,
		NAME_None,
		false,
		1.0f
	);
	ChargeMontageTask->ReadyForActivation();

	// 等待命中
	UAbilityTask_WaitGameplayEvent* WaitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this,
		ImpactEventTag,
		nullptr,
		false,
		false
	);
	WaitEventTask->EventReceived.AddDynamic(this, &UCP_Warrior_E::OnImpactEvent);
	WaitEventTask->ReadyForActivation();

	// 等待松键
	UAbilityTask_WaitInputRelease* WaitReleaseTask = UAbilityTask_WaitInputRelease::WaitInputRelease(
		this, false
	);
	WaitReleaseTask->OnRelease.AddDynamic(this, &UCP_Warrior_E::OnInputRelease);
	WaitReleaseTask->ReadyForActivation();
}

void UCP_Warrior_E::OnInputRelease(float TimeHeld)
{
	bChargeComplete = (TimeHeld >= ChargeTime);

	UAnimMontage* AttackMontage = bChargeComplete ? Montage_E_Full : Montage_E_Tap;

	UAbilityTask_PlayMontageAndWait* AttackMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		AttackMontage,
		1.0f,
		NAME_None,
		false,
		1.0f
	);
	AttackMontageTask->OnCompleted.AddDynamic(this, &UCP_Warrior_E::OnMontageEnd);
	AttackMontageTask->OnBlendOut.AddDynamic(this, &UCP_Warrior_E::OnMontageEnd);
	AttackMontageTask->ReadyForActivation();

	UE_LOG(LogTemp, Warning, TEXT("[Warrior_E] OnInputRelease: TimeHeld=%.2f, bChargeComplete=%d"),
		TimeHeld, bChargeComplete);
}

void UCP_Warrior_E::OnImpactEvent(FGameplayEventData Payload)
{
	if (bSweepExecuted) return;
	bSweepExecuted = true;

	AActor* Avatar = GetAvatarActorFromActorInfo();

	// 客户端只做表现，碰撞/伤害/击退由服务器权威计算
	if (!Avatar->HasAuthority()) return;

	// 碰撞检测
	float Radius = bChargeComplete ? FullRadius : TapRadius;
	TArray<AActor*> HitTargets = UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(
		Avatar,
		Radius,
		0.f,
		0.f,
		false
	);

	// 击退
	float LaunchAngle = bChargeComplete ? 90.f : 15.f;
	float Force = bChargeComplete ? FullLaunchForce : TapForce;

	UCP_BlueprintFunctionLibrary::ApplyKnockback(
		Avatar, HitTargets,
		Radius, Radius,
		Force,
		LaunchAngle,
		false
	);

	// 遍历目标分发伤害
	float DamageMultiplier = bChargeComplete ? 1.8f : 1.0f;
	for (AActor* Target : HitTargets)
	{
		float Damage = BaseDamage * DamageMultiplier;

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
}

void UCP_Warrior_E::OnMontageEnd()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	UE_LOG(LogTemp, Warning, TEXT("[Warrior_E] OnMontageEnd"));
}
