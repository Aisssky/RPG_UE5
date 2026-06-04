// Copyright Aisssky


#include "Player/CP_PlayerState.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"	

ACP_PlayerState::ACP_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UCP_AbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);

	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UCP_AttributeSet>("AttributeSet");	

}

UAbilitySystemComponent* ACP_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
