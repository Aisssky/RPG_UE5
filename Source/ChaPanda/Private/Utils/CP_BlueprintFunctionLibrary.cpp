// Copyright Aisssky


#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "InterchangeTranslatorBase.h"
#include "ToolContextInterfaces.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "Character/CP_BaseCharacter.h"
#include "Character/CP_EnemyCharacter.h"
#include "GameplayTags/CP_Tags.h"
#include "Engine/OverlapResult.h"
#include "Kismet/GameplayStatics.h"
#include "Chaos/Deformable/ChaosDeformableSolverProxy.h"

EHitDirection UCP_BlueprintFunctionLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{
	const float Dot = FVector::DotProduct(TargetForward, ToInstigator);
	if (Dot < -0.5f)
	{
		return EHitDirection::Back;
	}
	if(Dot < 0.5f)
	{
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);	
		if (Cross.Z < 0)
		{
			return EHitDirection::Left;
		}
		return EHitDirection::Right;
	}
	return EHitDirection::Forward;
}

FName UCP_BlueprintFunctionLibrary::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
		case EHitDirection::Left:
			return FName("HitReact_Left");
		case EHitDirection::Right:
			return FName("HitReact_Right");
		case EHitDirection::Forward:
			return FName("HitReact_Forward");
		case EHitDirection::Back:
			return FName("HitReact_Back");
		default: return FName("None");
	}
}

FClosestActorWithTagResult UCP_BlueprintFunctionLibrary::FindClosestActorWithTag(UObject* WorldContextObject, const FVector& Origin, const FName& Tag, float SearchRange)
{
	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(WorldContextObject, Tag, ActorsWithTag);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor))continue;
		ACP_BaseCharacter* BaseCharacter = Cast<ACP_BaseCharacter>(Actor);
		if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive()) continue;

		const float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
		if (ACP_EnemyCharacter* SearchingCharacter = Cast<ACP_EnemyCharacter>(WorldContextObject);IsValid(SearchingCharacter))
		{
			if (Distance > SearchingCharacter->SearchRange)continue;
		}
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	FClosestActorWithTagResult Result;
	Result.Actor = ClosestActor;
	Result.Distance = ClosestDistance;

	return Result;
}

void UCP_BlueprintFunctionLibrary::SendDamageEventToPlayers(TArray<AActor*> Targets, const TSubclassOf<UGameplayEffect>& DamageEffect,FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{

	for (AActor* Target : Targets)
	{
		SendDamageEventToPlayer(Target, DamageEffect, Payload, DataTag, Damage, EventTagOverride, OptionalParticleSystem);
	}

}

void UCP_BlueprintFunctionLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticleSystem)
{
	ACP_BaseCharacter* PlayerCharacter = Cast<ACP_BaseCharacter>(Target);
	if (!IsValid(PlayerCharacter)) return;
	if (!PlayerCharacter->IsAlive()) return;

	FGameplayTag EventTag;
	if (!EventTagOverride.MatchesTagExact(CP_Tags::None))
	{
		EventTag = EventTagOverride;
	}
	else
	{
		UCP_AttributeSet* AttributeSet = Cast<UCP_AttributeSet>(PlayerCharacter->GetAttributeSet());
		if (!IsValid(AttributeSet)) return;

		const bool bLethal = AttributeSet->GetHealth() - Damage <= 0.f;
		EventTag = bLethal ? CP_Tags::Events::Player::Death : CP_Tags::Events::Player::HitReact;
	}

	Payload.OptionalObject = OptionalParticleSystem;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(PlayerCharacter, EventTag, Payload);

	UAbilitySystemComponent* TargetASC = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC)) return;

	FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DataTag, -Damage);

	TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

TArray<AActor*> UCP_BlueprintFunctionLibrary::HotBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOssset, bool bDrawDebugs)
{
	if (!IsValid(AvatarActor))return TArray<AActor*>();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AvatarActor);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);

	return TArray<AActor*>();
}

void UCP_BlueprintFunctionLibrary::DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!IsValid(World)) return;

	DrawDebugSphere(World, HitBoxLocation, HitBoxRadius, 16, FColor::Red, false, 3.f);

	for (const FOverlapResult& Result : OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(World, DebugLocation, 30.f, 10, FColor::Green, false, 3.f);
		}
	}
}

TArray<AActor*> UCP_BlueprintFunctionLibrary::ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius, float OuterRadius, float LaunchForceMagnitude, float RotationAngle, bool bDrawDebugs)
{
	for (AActor* HitActor : HitActors)
	{
		ACharacter* HitCharacter = Cast<ACharacter>(HitActor);
		if (!IsValid(HitCharacter) || !IsValid(AvatarActor))return TArray<AActor*>();

		const FVector HitCharacterLocation = HitCharacter->GetActorLocation();
		const FVector AvatarLocation = AvatarActor->GetActorLocation();

		const FVector ToHitActor = HitCharacterLocation - AvatarLocation;
		const float Distance = FVector::Dist(AvatarLocation, HitCharacterLocation);

		float LaunchForce = 0.f;
		if (Distance > OuterRadius) continue;
		if (Distance <= InnerRadius)
		{
			LaunchForce = LaunchForceMagnitude;
		}
		else
		{
			const FVector2D FalloffRange(InnerRadius, OuterRadius);
			const FVector2D LaunchForceRange(LaunchForceMagnitude, 0.f);

			LaunchForce = FMath::GetMappedRangeValueClamped(FalloffRange, LaunchForceRange,Distance);
		}
		if (bDrawDebugs)GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("LaunchForce:%f"), LaunchForce));

		FVector KnockbackForce = ToHitActor.GetSafeNormal();
		KnockbackForce.Z = 0.f;

		const FVector Right = KnockbackForce.RotateAngleAxis(90.f, FVector::UpVector);
		KnockbackForce = KnockbackForce.RotateAngleAxis(-RotationAngle, Right) * LaunchForce;

		if (bDrawDebugs)
		{
			UWorld* World = GEngine->GetWorldFromContextObject(AvatarActor, EGetWorldErrorMode::LogAndReturnNull);
			DrawDebugDirectionalArrow(World, HitCharacterLocation, HitCharacterLocation + KnockbackForce, 100.f, FColor::Green, false, 3.f);

		}

		if (ACP_EnemyCharacter* EnemyCharacter = Cast<ACP_EnemyCharacter>(HitCharacter); IsValid(EnemyCharacter))
		{
			EnemyCharacter->StopMovementUntilLanded();
		}

		HitCharacter->LaunchCharacter(KnockbackForce, true, true);
	}
	return HitActors;
}
