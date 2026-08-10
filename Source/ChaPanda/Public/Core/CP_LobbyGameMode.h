// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CP_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CHAPANDA_API ACP_LobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ACP_LobbyGameMode();

	void StartGame(const FString& MapName);
	//youshihouhaizhenshifenbuqingshenmeshihoujiaogeiUPROPERTY^
	UFUNCTION(BlueprintCallable)
	int32 GetLockedPlayerCount();

	UFUNCTION(BlueprintCallable)
	bool CanStartGame();

protected:
	virtual void PostLogin(APlayerController* NewPlayer)override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController)override;
	virtual bool ReadyToStartMatch_Implementation()override;

};
