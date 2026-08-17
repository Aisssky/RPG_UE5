// Copyright Aisssky


#include "Character/CP_EnemyCharacter.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include "GameplayTags/CP_Tags.h"
#include "Core/CP_GameState.h"
#include "Core/CP_GameMode.h"
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

	UE_LOG(LogTemp, Warning, TEXT("[Enemy] BeginPlay body | Name=%s | HasAuthority=%d | NetMode=%d"),
		*GetName(), HasAuthority(), (int32)GetNetMode());

	if (!IsValid(GetAbilitySystemComponent()))return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	UE_LOG(LogTemp, Warning, TEXT("[Enemy]   -> InitAbilityActorInfo done, broadcasting OnASCInitialized"));
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();

	// All GE modifiers applied — clamp, mark initialized, broadcast
	UCP_AttributeSet* CP_AttributeSet = Cast<UCP_AttributeSet>(GetAttributeSet());
	if (IsValid(CP_AttributeSet) && !CP_AttributeSet->bAttributesInitialized)
	{
		CP_AttributeSet->bAttributesInitialized = true;
		CP_AttributeSet->SetHealth(FMath::Clamp(CP_AttributeSet->GetHealth(), 0.0f, CP_AttributeSet->GetMaxHealth()));
		CP_AttributeSet->SetMana(FMath::Clamp(CP_AttributeSet->GetMana(), 0.0f, CP_AttributeSet->GetMaxMana()));
		UE_LOG(LogTemp, Warning, TEXT("[Enemy] InitializeAttributes done, broadcasting OnAttributesInitialized (Health=%.1f, MaxHealth=%.1f)"),
			CP_AttributeSet->GetHealth(), CP_AttributeSet->GetMaxHealth());
		CP_AttributeSet->OnAttributesInitialized.Broadcast();
	}
	if (!IsValid(CP_AttributeSet)) return;

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CP_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
}

void ACP_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	if (HasAuthority()) {
		if (ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>())
			GS->AliveEnemyCount = FMath::Max(0, GS->AliveEnemyCount - 1);

		if (ACP_GameMode* GM = GetWorld()->GetAuthGameMode<ACP_GameMode>())
			GM->OnEnemyKilled();
	}
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