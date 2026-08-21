// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "CP_PlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FGameplayTag;	

UCLASS()
class CHAPANDA_API ACP_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	FGameplayTag GetSelectedHeroTag() const;
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Server_StartGame(const FString& MapName);
	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Server_SelectedHero(const FGameplayTag& HeroTag);

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Server_LockedInHero();

	UPROPERTY(EditDefaultsOnly, Category = "Cha|UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;   // 大厅 UI 蓝图类（WBP_Lobby）

	UPROPERTY()
	TObjectPtr<UUserWidget> LobbyWidgetInstance;

	UFUNCTION()
	void OnGamePhaseChanged(FGameplayTag OldPhase, FGameplayTag NewPhase);

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
protected:
	
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	UPROPERTY(EditDefaultsOnly,Category="Cha|Input|Abilities")
	TObjectPtr<UInputAction> QAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Abilities")
	TObjectPtr<UInputAction> RAction;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Input|Abilities")
	TObjectPtr<UInputAction> EAction;

	void Jump();
	void StopJumping();

	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	void PrimaryAttack();
	void ActivateAbility(const FGameplayTag& AbilityTag) const;
	void SecondaryAttack();
	void TertiaryAttack();

	void QAbility();
	void RAbility();

	void EAbility();
	void EAbilityReleased();

	bool IsAlive() const;


	//RPC
	UFUNCTION(Server,Reliable,WithValidation)
	void Server_ActivateAbility(const FGameplayTag& AbilityTag);

	UFUNCTION(Client,Reliable)
	void Client_NotifyAbilityRejected(const FGameplayTag& AbilityTag);


};
