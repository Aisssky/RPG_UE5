// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Warrior/CP_Warrior_R.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Utils/CP_BlueprintFunctionLibrary.h" 
#include "GameplayEffect.h"
#include "TimerManager.h"

void UCP_Warrior_R::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo)) {
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage_R, 1.f, NAME_None, false, 1.f);

	MontageTask->OnCompleted.AddDynamic(this, &UCP_Warrior_R::OnMontageEnd);
	MontageTask->OnBlendOut.AddDynamic(this, &UCP_Warrior_R::OnMontageEnd);

	MontageTask->ReadyForActivation();

	FGameplayEffectSpecHandle Spec = MakeOutgoingGameplayEffectSpec(SelfBuffEffectClass, 1.f);

	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());


	GetWorld()->GetTimerManager().SetTimer(
		AuraTickHandle, this, &UCP_Warrior_R::TickAura, AuraTickInterval, true);
	GetWorld()->GetTimerManager().SetTimer(DurationHandle, this, &UCP_Warrior_R::OnMontageEnd, BuffDuration, false);

}


void UCP_Warrior_R::TickAura()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();

	TArray<AActor*> OverlappedActors=UCP_BlueprintFunctionLibrary::HitBoxOverlapTest(Avatar, AuraRadius,0.f,0.f,false);

	for(AActor* Actor:OverlappedActors){
		if (Actor == Avatar)continue;
		if (Actor->ActorHasTag("Enemy"))continue;

		UAbilitySystemComponent* AllyASC =UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		if (AllyASC)
		{
			FGameplayEffectContextHandle Ctx = AllyASC->MakeEffectContext();
			FGameplayEffectSpecHandle Spec = AllyASC->MakeOutgoingSpec(AllyBuffEffectClass, 1.f, Ctx); 
				AllyASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
		}
	}

	
}

void UCP_Warrior_R::OnMontageEnd()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}