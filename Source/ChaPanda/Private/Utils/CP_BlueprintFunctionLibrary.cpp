// Copyright Aisssky


#include "Utils/CP_BlueprintFunctionLibrary.h"

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
