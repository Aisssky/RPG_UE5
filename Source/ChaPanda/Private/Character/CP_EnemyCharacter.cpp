// Copyright Aisssky


#include "Character/CP_EnemyCharacter.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "GameplayTags/CP_Tags.h"


ACP_EnemyCharacter::ACP_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	
	AbilitySystemComponent = CreateDefaultSubobject<UCP_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCP_AttributeSet>("AttributeSet");
}

void ACP_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* ACP_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACP_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void ACP_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;

	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
	}
}

void ACP_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent()))return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();

	UCP_AttributeSet* CP_AttributeSet = Cast<UCP_AttributeSet>(GetAttributeSet());
	if (!IsValid(CP_AttributeSet)) return;

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CP_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACP_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();
}

void ACP_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CP_Tags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);	
}
