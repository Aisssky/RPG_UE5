// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h" 
#include "CP_Projectile.generated.h"

class UProjectileMovementComponent;
class UGameplayEffect;

UCLASS()
class CHAPANDA_API ACP_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACP_Projectile();
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "Cha|Damage",meta=(ExposeOnSpawn,ClampMin="0.0"))
	float Damage{ 25.f };

	//扩展
	UFUNCTION(BlueprintCallable, Category = "Cha|Projectile")
	void SetHomingTarget(AActor* Target);


	UFUNCTION(BlueprintImplementableEvent,Category="Cha|Projectile")
	void SpawnImpactEffects();

private:	

	UPROPERTY(VisibleAnywhere, Category = "Cha|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;


	//区分
	UPROPERTY(EditDefaultsOnly, Category = "Cha|Damage")
	FGameplayTag SetByCallerTag;

};
