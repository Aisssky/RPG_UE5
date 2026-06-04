// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CP_MeleeAttack.generated.h"

UCLASS()
class CHAPANDA_API UCP_MeleeAttack : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:

	UPROPERTY(EditAnywhere, Category = "Cha|Debugs")
	bool bDrawDebugs = true;

	UPROPERTY(EditAnywhere, Category = "Cha|Socket")
	FName SocketName{ "FX_Trail_01_R" };

	UPROPERTY(EditAnywhere, Category = "Cha|Socket")
	float SocketExtensionOffset{ 40.f };

	UPROPERTY(EditAnywhere, Category = "Cha|Socket")
	float SphereTraceRadius{ 60.f };

	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp)const;
	void SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits)const;
};