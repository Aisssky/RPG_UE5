// Copyright Aisssky


#include "UI/CP_WidgetComponent.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "UI/CP_AttributeWidget.h"
#include "Character/CP_BaseCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogCPWidget, Log, All);

void UCP_WidgetComponent::InitAbilitySystemData()
{
	ChaCharacter = Cast<ACP_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UCP_AttributeSet>(ChaCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UCP_AbilitySystemComponent>(ChaCharacter->GetAbilitySystemComponent());

	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] InitAbilitySystemData | Owner=%s | ASC=%s | AS=%s | NetMode=%d"),
		*GetOwner()->GetName(),
		AbilitySystemComponent.IsValid() ? TEXT("Valid") : TEXT("NULL"),
		AttributeSet.IsValid() ? TEXT("Valid") : TEXT("NULL"),
		(int32)GetOwner()->GetNetMode());
}

bool UCP_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCP_WidgetComponent::InitializeAttributeDelegete()
{
	const bool bInit = AttributeSet.IsValid() && AttributeSet->bAttributesInitialized;
	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] InitializeAttributeDelegete | Owner=%s | bAttributesInitialized=%d | HasAuthority=%d"),
		*GetOwner()->GetName(), bInit, GetOwner()->HasAuthority());

	if (!AttributeSet->bAttributesInitialized)
	{
		UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget]   -> Binding to OnAttributesInitialized delegate (waiting for replication)"));
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);

	}
	else
	{
		UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget]   -> bAttributesInitialized already true, calling BindToAttributeChanges directly"));
		BindToAttributeChanges();
	}
}

void UCP_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
		UCP_AttributeWidget* AttributeWidget = Cast<UCP_AttributeWidget>(WidgetObject);
		if (!IsValid(AttributeWidget))
		{
			UE_LOG(LogCPWidget, Verbose, TEXT("[CPWidget] BindWidgetToAttributeChanges | WidgetObject is not UCP_AttributeWidget, skipping"));
			return;
		}
		if (!AttributeWidget->MatchAttributes(Pair))
		{
			UE_LOG(LogCPWidget, Verbose, TEXT("[CPWidget] BindWidgetToAttributeChanges | Attribute mismatch, skipping widget"));
			return;
		}

		const float CurValue = Pair.Key.GetNumericValue(AttributeSet.Get());
		const float MaxValue = Pair.Value.GetNumericValue(AttributeSet.Get());
		UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] BindWidgetToAttributeChanges | Owner=%s | Attribute=%s | CurValue=%.1f | MaxValue=%.1f"),
			*GetOwner()->GetName(), *Pair.Key.GetName(), CurValue, MaxValue);

		AttributeWidget->AvatarActor = ChaCharacter;

		AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), 0.0f);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			const float NewValue = Pair.Key.GetNumericValue(AttributeSet.Get());
			const float MaxValue = Pair.Value.GetNumericValue(AttributeSet.Get());
			UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] Lambda OnAttributeChange | Owner=%s | Attribute=%s | NewValue=%.1f | MaxValue=%.1f | OldValue=%.1f"),
				*GetOwner()->GetName(), *Pair.Key.GetName(), NewValue, MaxValue, AttributeChangeData.OldValue);
			AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(),AttributeChangeData.OldValue);
		});
}

void UCP_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] OnASCInitialized | Owner=%s"), *GetOwner()->GetName());

	AbilitySystemComponent = Cast<UCP_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCP_AttributeSet>(AS);

	if (!IsASCInitialized())
	{
		UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget]   -> ASC/AS cast failed, returning"));
		return;
	}
	InitializeAttributeDelegete();
}

void UCP_WidgetComponent::BindToAttributeChanges()
{
	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] BindToAttributeChanges | Owner=%s | AttributeMap entries=%d | WidgetObject=%s"),
		*GetOwner()->GetName(),
		AttributeMap.Num(),
		GetUserWidgetObject() ? *GetUserWidgetObject()->GetName() : TEXT("NULL"));

	if (!GetUserWidgetObject())
	{
		UE_LOG(LogCPWidget, Error, TEXT("[CPWidget]   -> GetUserWidgetObject() is NULL! Widget Class may not be set!"));
		return;
	}

	if (!GetUserWidgetObject()->WidgetTree)
	{
		UE_LOG(LogCPWidget, Error, TEXT("[CPWidget]   -> WidgetTree is NULL!"));
		return;
	}

	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair);

		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, &Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});
	}
}



void UCP_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget] BeginPlay | Owner=%s | HasAuthority=%d | NetMode=%d"),
		*GetOwner()->GetName(), GetOwner()->HasAuthority(), (int32)GetOwner()->GetNetMode());

	InitAbilitySystemData();

	if(!IsASCInitialized())
	{
		UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget]   -> ASC not initialized yet, binding to OnASCInitialized delegate"));
		ChaCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		return;
	}

	UE_LOG(LogCPWidget, Warning, TEXT("[CPWidget]   -> ASC/AS already valid, calling InitializeAttributeDelegete"));
	InitializeAttributeDelegete();
}
