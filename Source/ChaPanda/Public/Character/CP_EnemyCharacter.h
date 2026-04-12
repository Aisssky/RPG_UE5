// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Character/CP_BaseCharacter.h"
#include "CP_EnemyCharacter.generated.h"

class UAbilitySystemComponent;

UCLASS()
class CHAPANDA_API ACP_EnemyCharacter : public ACP_BaseCharacter
{
	GENERATED_BODY()
	
public:
	ACP_EnemyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

};
