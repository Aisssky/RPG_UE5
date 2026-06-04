// Copyright Aisssky


#include "UI/CP_WidgetComponent.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "UI/CP_AttributeWidget.h"
#include "Character/CP_BaseCharacter.h"

void UCP_WidgetComponent::InitAbilitySystemData()
{
	ChaCharacter = Cast<ACP_BaseCharacter>(GetOwner());
	AttributeSet = Cast<UCP_AttributeSet>(ChaCharacter->GetAttributeSet());
	AbilitySystemComponent = Cast<UCP_AbilitySystemComponent>(ChaCharacter->GetAbilitySystemComponent());
}

bool UCP_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCP_WidgetComponent::InitializeAttributeDelegete()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);

	}
	else
	{
		BindToAttributeChanges();
	}
}

void UCP_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
		UCP_AttributeWidget* AttributeWidget = Cast<UCP_AttributeWidget>(WidgetObject);
		if (!IsValid(AttributeWidget))return;
		if (!AttributeWidget->MatchAttributes(Pair))return;

		AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), 0.0f);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, AttributeWidget, &Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(),AttributeChangeData.OldValue);
		});
}

void UCP_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	AbilitySystemComponent = Cast<UCP_AbilitySystemComponent>(ASC);
	AttributeSet = Cast<UCP_AttributeSet>(AS);

	if (!IsASCInitialized())return;
	InitializeAttributeDelegete();
}

void UCP_WidgetComponent::BindToAttributeChanges()
{
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

	InitAbilitySystemData();

	if(!IsASCInitialized())
	{
		ChaCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		return;
	}

	InitializeAttributeDelegete();
}
