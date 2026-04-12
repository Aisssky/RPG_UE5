// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

#include "CP_BaseCharacter.generated.h"

class UGameplayAbility;

UCLASS(Abstract)
class CHAPANDA_API ACP_BaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ACP_BaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	void GiveStartupAbilities();

private:

	UPROPERTY(EditAnywhere, Category = "Cha|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
