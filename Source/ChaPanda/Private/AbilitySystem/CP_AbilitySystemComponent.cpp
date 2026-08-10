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

void UCP_AbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
    if(!IsValid(GetAvatarActor())||!GetAvatarActor()->HasAuthority()) return;

    if(FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
    {
        AbilitySpec->Level = Level;
		MarkAbilitySpecDirty(*AbilitySpec); 
    }
}

void UCP_AbilitySystemComponent::AddToAbilityLevel(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level)
{
    if(!IsValid(GetAvatarActor())||!GetAvatarActor()->HasAuthority()) return;
    if(FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromClass(AbilityClass))
    {
        AbilitySpec->Level += Level;
        MarkAbilitySpecDirty(*AbilitySpec); 
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
            return;
        }
    }
}

void UCP_AbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputPressed(Spec);
    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());

}

void UCP_AbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
    Super::AbilitySpecInputReleased(Spec);
    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
}