// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Character/CP_BaseCharacter.h"
#include "CP_EnemyCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class CHAPANDA_API ACP_EnemyCharacter : public ACP_BaseCharacter
{
	GENERATED_BODY()

public:
	ACP_EnemyCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cha|AI")
	float AcceptanceRadius{ 500.f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cha|AI")
	float MinAttackDelay{ .1f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cha|AI")
	float MaxAttackDelay{ .5f };

	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cha|AI")
	bool bIsBeingLaunched{ false };

	void StopMovementUntilLanded();

protected:
	virtual void BeginPlay() override;
	virtual void HandleDeath() override;

private:

	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& Hit);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


};
