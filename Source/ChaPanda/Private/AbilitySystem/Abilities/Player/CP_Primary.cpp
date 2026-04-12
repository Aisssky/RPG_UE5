// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/CP_Primary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CP_Tags.h"

TArray<AActor*> UCP_Primary::HitBoxOverlapTest()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

	TArray<FOverlapResult> OverlapsResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0.0f, 0.0f, HitBoxElevationOffset);

	GetWorld()->OverlapMultiByChannel(OverlapsResults, HitBoxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	TArray<AActor*> ActorsHit;
	for (const FOverlapResult& Result : OverlapsResults)
	{
		if (!IsValid(Result.GetActor()))continue;
		ActorsHit.AddUnique(Result.GetActor());
	}
	

	if (bDrawDebugs)
	{
		DrawHitBoxDebug(OverlapsResults, HitBoxLocation);
	}

	return ActorsHit;
}

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

void UCP_Primary::DrawHitBoxDebug(const TArray<FOverlapResult>& OverlapsResults,const FVector& HitBoxLocation) const
{
	DrawDebugSphere(GetWorld(), HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.0f);

	for (const FOverlapResult& Result : OverlapsResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(GetWorld(), DebugLocation, 10.0f, 8, FColor::Blue, false, 3.0f);
		}
	}
}
