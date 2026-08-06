// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameplayTagContainer.h"
#include "CP_GameMode.generated.h"

UCLASS()
class CHAPANDA_API ACP_GameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACP_GameMode();
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<APawn>> HeroPawnClasses;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
