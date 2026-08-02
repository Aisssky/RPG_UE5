// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CP_GameplayAbility.h"
#include "CP_Primary.generated.h"


UCLASS()
class CHAPANDA_API UCP_Primary : public UCP_GameplayAbility
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Cha|Abilities")
	void SendHitReactEventToActors(const TArray<AActor*>& ActorsHit);

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Cha|Abilities")
	float HitBoxRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Abilities")
	float HitBoxForwardOffset = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Abilities")
	float HitBoxElevationOffset = 20.0f;

	void DrawHitBoxDebug(const TArray<FOverlapResult>& OverlapsResults,const FVector& HitBoxLocation) const; 

};
