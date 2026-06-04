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

FCloseActorWithTagResult UCP_BlueprintFunctionLibrary::FindClosestActorWithTag(UObject* WorldContextObject, const FVector& Origin, const FName& Tag, float SearchRange)
{
	return FCloseActorWithTagResult();
}

void UCP_BlueprintFunctionLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect, UPARAM(ref)FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage, const FGameplayTag& EventTagOverride, UObject* OptionalParticalSystem)
{
}

TArray<AActor*> UCP_BlueprintFunctionLibrary::HotBoxOverlapTest(AActor* AvatarActor, float HitBoxRadius, float HitBoxForwardOffset, float HitBoxElevationOssset, bool bDrawDebugs)
{
	return TArray<AActor*>();
}

void UCP_BlueprintFunctionLibrary::DrawHitBoxOverlapDebugs(const UObject* WorldContextObject, const TArray<FOverlapResult>& OverlapResults, const FVector& HitBoxLocation, float HitBoxRadius)
{
}

TArray<AActor*> UCP_BlueprintFunctionLibrary::ApplyKnockback(AActor* AvatarActor, const TArray<AActor*>& HitActors, float InnerRadius, float OuterRadius, float RotationAngle, bool bDrawDebugs)
{
	return TArray<AActor*>();
}
