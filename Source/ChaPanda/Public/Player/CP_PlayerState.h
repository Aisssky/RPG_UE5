// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"
#include "CP_PlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class CHAPANDA_API ACP_PlayerState : public APlayerState, public IAbilitySystemInterface  
{
	GENERATED_BODY()
	
public:

	ACP_PlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetSelectedHeroTag()const { return SelectedHeroTag; }

	UFUNCTION(BlueprintCallable)
	bool IsHeroLocked() const { return bHeroLocked; }

	UFUNCTION(BlueprintCallable)
	int32 GetKills()const { return KillCount; }
 
	UFUNCTION(BlueprintCallable)
	int32 GetDeaths()const { return DeathCount; }

	UFUNCTION(BlueprintCallable)
	int32 GetAssists()const { return AssistCount; }


	UPROPERTY(VisibleAnywhere, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(ReplicatedUsing=OnRep_SelectedHeroTag)
	FGameplayTag SelectedHeroTag;

	UFUNCTION()
	void OnRep_SelectedHeroTag();

	UPROPERTY(BlueprintReadOnly,Replicated)
	bool bHeroLocked;

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_SelectHero(const FGameplayTag& HeroTag);

	UFUNCTION(Server,Reliable,WithValidation)
	void Server_LockInHero();

	void AddKill() { ++KillCount; }
	void AddDeath() { ++DeathCount; }
	void AddAssist() { ++AssistCount; }


private:
	UPROPERTY(Replicated,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	int32 KillCount=0;

	UPROPERTY(Replicated,BlueprintReadOnly,meta=(AllowPrivateAccess="true"))
	int32 DeathCount = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 AssistCount = 0;

};
