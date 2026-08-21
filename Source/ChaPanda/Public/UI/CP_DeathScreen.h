// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemComponent.h"
#include "CP_DeathScreen.generated.h"

class UTextBlock;

UCLASS()
class CHAPANDA_API UCP_DeathScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable,Category="Cha|Death")
	void StartCountdown(float Seconds);

protected:
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;


	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CountdownText;

	UPROPERTY(EditDefaultsOnly,Category="Cha|Death")
	float RespawnDelay = 5.f;

private:

	FTimerHandle CountdownTimerHandle;
	float RemainingTime = 0.f;

	UAbilitySystemComponent* GetOwningASC() const;

	void TickCountdown();

	UFUNCTION()
	void OnDeadTagChanged(const FGameplayTag Tag, int32 NewCount);

};
