// Copyright Aisssky


#include "GameObject/CP_Projectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/CP_Tags.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"

ACP_Projectile::ACP_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	
	bReplicates = true;

}

void ACP_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACP_PlayerCharacter* PlayerCharacter = Cast<ACP_PlayerCharacter>(OtherActor);
	if(!IsValid(PlayerCharacter))return;
	if (!PlayerCharacter->IsAlive())return;

	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (IsValid(AbilitySystemComponent) || !HasAuthority())return;	

	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = PlayerCharacter;

	UCP_BlueprintFunctionLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, CP_Tags::SetByCaller::Projectile, Damage, CP_Tags::None);

	SpawnImpactEffects();
	Destroy();
}