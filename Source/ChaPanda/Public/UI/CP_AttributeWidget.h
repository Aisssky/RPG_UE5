// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "CP_AttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHAPANDA_API UCP_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cha|Attributes")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cha|Attributes")
	FGameplayAttribute MaxAttribute;

	void OnAttributeChange(const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair, UCP_AttributeSet* AttributeSet,float OldValue);	

	bool MatchAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

	UFUNCTION(BlueprintImplementableEvent,meta = (DisplayName = "On Attribute Changed"))
	void BP_OnAttributeChanged(float NewValue, float NewMaxValue,float OldValue);	

};
