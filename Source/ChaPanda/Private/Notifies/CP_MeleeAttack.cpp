// Copyright Aisssky


#include "Notifies/CP_MeleeAttack.h"

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
