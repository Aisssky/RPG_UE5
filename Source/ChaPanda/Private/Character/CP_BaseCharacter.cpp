// Copyright Aisssky


#include "Character/CP_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

namespace ChaTags
{
	const FName Player = FName("Player");
}

ACP_BaseCharacter::ACP_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

}

void ACP_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACP_BaseCharacter, bAlive);
}
 
UAbilitySystemComponent* ACP_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

void ACP_BaseCharacter::OnHealthChanged(const FOnAttributeChangeData& AttributeChangeData)
{
}

void ACP_BaseCharacter::HandleDeath()
{
	bAlive = false;
}

void ACP_BaseCharacter::HandleRespawn()
{
	bAlive = true;
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

void ACP_BaseCharacter::InitializeAttributes() const
{
	checkf(IsValid(InitializeAttributesEffect),TEXT("InitializeAttributesEffect is not valid"));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributesEffect, 1.f, ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACP_BaseCharacter::ResetAttributes()
{
	checkf(IsValid(ResetAttributesEffect), TEXT("ResetAttributesEffect is not valid"));

	if(!IsValid(GetAbilitySystemComponent())) return;

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(ResetAttributesEffect, 1.f, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}