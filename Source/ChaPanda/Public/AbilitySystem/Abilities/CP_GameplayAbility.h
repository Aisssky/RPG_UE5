// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CP_GameplayAbility.generated.h"


UCLASS()
class CHAPANDA_API UCP_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cha|Debug")
	bool bDrawDebugs = false;
};
