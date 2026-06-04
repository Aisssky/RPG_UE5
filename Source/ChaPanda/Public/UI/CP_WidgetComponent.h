// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "AttributeSet.h"
#include "Character/CP_BaseCharacter.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "AbilitySystem/CP_AttributeSet.h"
#include "CP_WidgetComponent.generated.h"

class UAbilitySystemComponent;
class ACP_BaseCharacter;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CHAPANDA_API UCP_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:


protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayAttribute, FGameplayAttribute> AttributeMap;

private:

	TWeakObjectPtr<ACP_BaseCharacter> ChaCharacter;
	TWeakObjectPtr<UCP_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCP_AttributeSet> AttributeSet;

	
	void InitAbilitySystemData();
	bool IsASCInitialized() const ; 
	void InitializeAttributeDelegete();
	void BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

	UFUNCTION()
	void OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UFUNCTION()
	void BindToAttributeChanges();



};
