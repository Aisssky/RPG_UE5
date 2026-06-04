// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/CP_Primary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"

#include "GameplayTags/CP_Tags.h"

void UCP_Primary::SendHitReactEventToActors(const TArray<AActor*>& ActorsHit)
{
	for (AActor* Actor : ActorsHit)
	{
		if (!IsValid(Actor))continue;

		FGameplayEventData Payload;
		Payload.Instigator = GetAvatarActorFromActorInfo();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Actor, CP_Tags::Events::Enemy::HitReact, Payload);
	}
}