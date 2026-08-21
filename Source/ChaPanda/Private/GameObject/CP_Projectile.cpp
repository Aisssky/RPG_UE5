// Copyright Aisssky


#include "GameObject/CP_Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_BaseCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/CP_Tags.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"

ACP_Projectile::ACP_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	
	bReplicates = true;

	SetByCallerTag = CP_Tags::SetByCaller::Projectile;
}

void ACP_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACP_BaseCharacter* HitCharacter = Cast<ACP_BaseCharacter>(OtherActor);
	if (!IsValid(HitCharacter)) return;
	if (HitCharacter == GetOwner()) return;
	if (!HitCharacter->IsAlive()) return;

	if (!HasAuthority()) return;

	UAbilitySystemComponent* AbilitySystemComponent = HitCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent) || !HasAuthority())return;	

	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = HitCharacter;

	UCP_BlueprintFunctionLibrary::SendDamageEventToPlayer(HitCharacter, DamageEffect, Payload,SetByCallerTag, Damage, CP_Tags::None);

	SpawnImpactEffects();
	Destroy();
}

void ACP_Projectile::SetHomingTarget(AActor* Target)
{
	if (!IsValid(Target) || !IsValid(ProjectileMovement)) return;

	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
}
