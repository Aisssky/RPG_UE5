// Copyright Aisssky


#include "Player/CP_PlayerState.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"	
#include "GameplayTags/CP_Tags.h"
#include "Core/CP_GameState.h"

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

void ACP_PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACP_PlayerState, SelectedHeroTag);
	DOREPLIFETIME(ACP_PlayerState, bHeroLocked);
	DOREPLIFETIME(ACP_PlayerState, KillCount);
	DOREPLIFETIME(ACP_PlayerState, DeathCount);
	DOREPLIFETIME(ACP_PlayerState, AssistCount);
}

void ACP_PlayerState::OnRep_SelectedHeroTag(FGameplayTag OldTag)
{
	OnHeroSelectionChanged.Broadcast(OldTag, SelectedHeroTag);
}

void ACP_PlayerState::OnRep_bHeroLocked()
{
	OnHeroLockChanged.Broadcast(bHeroLocked);
}

bool ACP_PlayerState::Server_SelectHero_Validate(const FGameplayTag& HeroTag)
{
	return HeroTag.IsValid();
}

void ACP_PlayerState::Server_SelectHero_Implementation(const FGameplayTag& HeroTag)
{
	if (ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>()) {
		if (!GS->IsInPhase(CP_Tags::GamePhase::CharacterSelect))return;
		if (bHeroLocked)return;

		for (APlayerState* OtherPS :GS->PlayerArray) {
			if (OtherPS == this)continue;
			ACP_PlayerState* OtherCPS = Cast<ACP_PlayerState>(OtherPS);
			if (OtherCPS && OtherCPS->IsHeroLocked() && OtherCPS->GetSelectedHeroTag() == HeroTag)
			{
				return;
			}
		}

		FGameplayTag OldTag=SelectedHeroTag;
		SelectedHeroTag = HeroTag;
		OnHeroSelectionChanged.Broadcast(OldTag, SelectedHeroTag);
	}
}

bool ACP_PlayerState::Server_LockInHero_Validate()
{
	return SelectedHeroTag.IsValid()&&!bHeroLocked;
}

void ACP_PlayerState::Server_LockInHero_Implementation()
{
	if (!SelectedHeroTag.IsValid())return;
	if (bHeroLocked)return;

	bHeroLocked=true;
	OnHeroLockChanged.Broadcast(true);
}