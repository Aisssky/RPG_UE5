// Copyright Aisssky


#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "GameplayTags/CP_Tags.h"


void UCP_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
    Super::OnGiveAbility(AbilitySpec);

    HandleActivateAbility(AbilitySpec);
}

void UCP_AbilitySystemComponent::OnRep_ActivateAbilities()
{
    Super::OnRep_ActivateAbilities();

	FScopedAbilityListLock AbilityListLock(*this);
    for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
    {
        HandleActivateAbility(AbilitySpec);
	}
}

void UCP_AbilitySystemComponent::HandleActivateAbility(const FGameplayAbilitySpec& AbilitySpec)
{
    if (!IsValid(AbilitySpec.Ability)) return;

    for (const FGameplayTag& Tag : AbilitySpec.Ability->GetAssetTags())
    {
        if (Tag.MatchesTagExact(CP_Tags::CPAbilities::ActivateOnGiven))
        {
            TryActivateAbility(AbilitySpec.Handle);
        }
    }
}
