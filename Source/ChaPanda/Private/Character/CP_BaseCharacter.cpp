// Copyright Aisssky


#include "Character/CP_BaseCharacter.h"
#include "AbilitySystemComponent.h"

ACP_BaseCharacter::ACP_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

}
 
UAbilitySystemComponent* ACP_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void ACP_BaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority())return;
	for(const auto& Ability : StartupAbilities)
	{
		if (!IsValid(Ability))return;
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}
