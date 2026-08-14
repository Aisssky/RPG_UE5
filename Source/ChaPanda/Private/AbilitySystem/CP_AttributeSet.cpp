// Copyright Aisssky


#include "AbilitySystem/CP_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/CP_Tags.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogCPAttributeSet, Log, All);

void UCP_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CritRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CritDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, DamageReduction, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(ThisClass, bAttributesInitialized);
}

void UCP_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// Only clamp after initialization — during init, MaxHealth may not be set yet
	if (bAttributesInitialized)
	{
		if(Data.EvaluatedData.Attribute == GetHealthAttribute())
		{
			SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		}

		if(Data.EvaluatedData.Attribute == GetManaAttribute())
		{
			SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
		}
	}

	if(Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth() <= 0.f)
	{
		FGameplayEventData Payload;
		Payload.Instigator = Data.Target.GetAvatarActor();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.EffectSpec.GetEffectContext().GetInstigator(), CP_Tags::Events::KillScored, Payload);
	}
}

void UCP_AttributeSet::OnRep_AttributesInitialized()
{
	UE_LOG(LogCPAttributeSet, Warning, TEXT("[CPAttributeSet] OnRep_AttributesInitialized | bAttributesInitialized=%d | Owner=%s"),
		bAttributesInitialized,
		GetOwningActor() ? *GetOwningActor()->GetName() : TEXT("NULL"));

	if(bAttributesInitialized)
	{
		UE_LOG(LogCPAttributeSet, Warning, TEXT("[CPAttributeSet]   -> Broadcasting OnAttributesInitialized"));
		OnAttributesInitialized.Broadcast();
	}
}

void UCP_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	UE_LOG(LogCPAttributeSet, Verbose, TEXT("[CPAttributeSet] OnRep_Health | Old=%.1f New=%.1f | Owner=%s"),
		OldValue.GetCurrentValue(), Health.GetCurrentValue(),
		GetOwningActor() ? *GetOwningActor()->GetName() : TEXT("NULL"));
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}

void UCP_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void UCP_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue);
}

void UCP_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldValue);
}

void UCP_AttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UCP_AttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defense, OldValue);
}

void UCP_AttributeSet::OnRep_CritRate(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CritRate, OldValue);
}

void UCP_AttributeSet::OnRep_CritDamage(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CritDamage, OldValue);
}

void UCP_AttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MoveSpeed, OldValue);
}

void UCP_AttributeSet::OnRep_DamageReduction(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, DamageReduction, OldValue);
}

