// Copyright Aisssky


#include "Notifies/CP_MeleeAttack.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "KismetTraceUtils.h"
#include "Character/CP_PlayerCharacter.h"
#include "GameplayTags/CP_Tags.h"
#include "Kismet/KismetMathLibrary.h"


void UCP_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{

}

TArray<FHitResult> UCP_MeleeAttack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
    return TArray<FHitResult>();
}

void UCP_MeleeAttack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits) const
{
}
