// Copyright Aisssky


#include "AbilitySystem/Abilities/Enemy/CP_HitReact.h"

void UCP_HitReact::CachedHitDirectionVectors(AActor* Instigator)
{
	AvatorForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();

	const FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector InstigatorLocation = Instigator->GetActorLocation();

	ToInstigator = InstigatorLocation - AvatarLocation;
	ToInstigator.Normalize();

}
