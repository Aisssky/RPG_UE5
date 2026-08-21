// Copyright Aisssky


#include "AbilitySystem/Abilities/Player/Archer/CP_Archer_Primary.h"
#include "Utils/CP_BlueprintFunctionLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameObject/CP_Projectile.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_EnemyCharacter.h"

void UCP_Archer_Primary::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FireArrow();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UCP_Archer_Primary::FireArrow()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();

	if (!IsValid(Avatar)) return;
	if (!Avatar->HasAuthority()) return;

	ACP_EnemyCharacter* Enemy = Cast<ACP_EnemyCharacter>(Avatar);
	if (!IsValid(Enemy))return;


	const FVector& Origin = Avatar->GetActorLocation();
	float SearchRange = 100.f + Enemy->AcceptanceRadius;

	FClosestActorWithTagResult  LockResult= UCP_BlueprintFunctionLibrary::FindClosestActorWithTag(Avatar, Avatar->GetActorLocation(), FName("Minion"), LockRange);

	if (!LockResult.Actor.IsValid() || LockResult.Distance > LockRange) return;

	AActor* Target = LockResult.Actor.Get();

	const FVector SpawnLocation = Avatar->GetActorLocation() + FVector(0.f, 0.f, 100.f);
	const FRotator SpawnRotation = (Target->GetActorLocation() - SpawnLocation).Rotation();
	const FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	ACP_Projectile* Arrow = GetWorld()->SpawnActorDeferred<ACP_Projectile>(
	ProjectileClass, SpawnTransform, Avatar, Cast<APawn>(Avatar),
	ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (!IsValid(Arrow)) return;

	Arrow->SetHomingTarget(Target);
	Arrow->Damage = BaseDamage;
	Arrow->FinishSpawning(SpawnTransform);
}
