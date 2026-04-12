// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "CP_PlayerState.generated.h"

class UAbilitySystemComponent;

UCLASS()
class CHAPANDA_API ACP_PlayerState : public APlayerState, public IAbilitySystemInterface  
{
	GENERATED_BODY()
	
public:
	ACP_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
