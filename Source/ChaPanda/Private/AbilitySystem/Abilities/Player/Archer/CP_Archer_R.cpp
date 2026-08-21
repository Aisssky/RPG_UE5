// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Archer/CP_Archer_R.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UCP_Archer_R::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	//冷却检查
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Detonate();
}

void UCP_Archer_R::Detonate()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	FVector Center = Avatar->GetActorLocation();

	if (!Avatar->HasAuthority()) return;
	TArray<AActor*> HitTargets = UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(
		Avatar,
		DetonateRadius,
		0.f,
		0.f,
		false
	);

	for (AActor* Target : HitTargets) {
		UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
		int32 Stacks = ASC->GetTagCount(CP_Tags::Status::PoisonMark);

		if (Stacks > 0) {
			float Damage = BaseDamage * Stacks;
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
			const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
				FGameplayTagContainer(CP_Tags::Status::PoisonMark));
			ASC->RemoveActiveEffects(Query);

		}
		
		}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
