// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Character/CP_BaseCharacter.h"
#include "CP_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class CHAPANDA_API ACP_PlayerCharacter : public ACP_BaseCharacter
{
	GENERATED_BODY()
	
public:
	ACP_PlayerCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;


private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;


};
