// Copyright Aisssky


#include "AbilitySystem/Abilities/CP_Death.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_BaseCharacter.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/PlayerStart.h"

void UCP_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	bHasRespawned = false;

	Super::ActivateAbility(Handle,ActorInfo, ActivationInfo, TriggerEventData);
	if (!Montage_Death) {
		RespawnCharacter(); return;
	}

	UAbilityTask_PlayMontageAndWait* DeathMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		Montage_Death,
		1.0f,
		NAME_None,
		false,
		1.0f
	);


	DeathMontageTask->OnCompleted.AddDynamic(this, &UCP_Death::OnDeathMontageEnded);
	DeathMontageTask->OnBlendOut.AddDynamic(this, &UCP_Death::OnDeathMontageEnded);

	DeathMontageTask->ReadyForActivation();

	
}

void UCP_Death::OnDeathMontageEnded()
{
	if (bHasRespawned)return;
	bHasRespawned = true;

	if (!HasAuthority(&CurrentActivationInfo)) {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}


	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	FGameplayEffectContextHandle Ctx = ASC->MakeEffectContext();
	if (DeathEffect) {
		FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(
			DeathEffect, 1.f, Ctx
		);
		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	if (ACP_BaseCharacter* BC = Cast<ACP_BaseCharacter>(GetAvatarActorFromActorInfo())) {
		BC->SetActorHiddenInGame(true);//隐藏
		BC->SetActorEnableCollision(false);//尸体不挡路
	}
	GetWorld()->GetTimerManager().SetTimer(
		DeathTimerHandle, this, &UCP_Death::RespawnCharacter, RespawnDelay, false
	);
}

void UCP_Death::RespawnCharacter()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponentFromActorInfo();
	AActor* Avatar = GetAvatarActorFromActorInfo();
	ACP_BaseCharacter* BaseCharacter = Cast<ACP_BaseCharacter>(Avatar);

	if (!ASC)return;

	if (!HasAuthority(&CurrentActivationInfo)) {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(FGameplayTagContainer(CP_Tags::Status::Dead));
	ASC->RemoveActiveEffects(Query);

	ASC->CancelAbilities();
	if (!IsValid(BaseCharacter)) {
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}
	
	BaseCharacter->SetActorHiddenInGame(false);
	BaseCharacter->SetActorEnableCollision(true);

	BaseCharacter->ResetAttributes();

	if (AController* PC = BaseCharacter->GetController()) {
		UWorld* World = GetWorld();
		if (!World)return;
		AGameModeBase* GM = World->GetAuthGameMode();
		if (!GM)return;

		AActor* PlayerStart = GM->FindPlayerStart(PC);
		if (!PlayerStart)return;

		FVector NewLocation = PlayerStart->GetActorLocation();
		BaseCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::ResetPhysics);

		
	}

	BaseCharacter->HandleRespawn();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
