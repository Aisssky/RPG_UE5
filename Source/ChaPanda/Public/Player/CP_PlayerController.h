// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CP_PlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FGameplayTag;	

UCLASS()
class CHAPANDA_API ACP_PlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void SetupInputComponent() override;
private:
	 
	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input")
	TArray<TObjectPtr<UInputMappingContext>> InputMappingContexts;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Movement")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Movement")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Movement")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Abilities")
	TObjectPtr<UInputAction> PrimaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Abilities")
	TObjectPtr<UInputAction> SecondaryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Abilities")
	TObjectPtr<UInputAction> TertiaryAction;


	void Jump();
	void StopJumping();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void PrimaryAttack();
	void ActivateAbility(const FGameplayTag& AbilityTag) const;
	void SecondaryAttack();
	void TertiaryAttack();
};
