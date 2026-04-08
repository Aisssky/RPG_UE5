// Copyright Aisssky


#include "Player/CP_PlayerState.h"
#include "AbilitySystemComponent.h"

ACP_PlayerState::ACP_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ACP_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
