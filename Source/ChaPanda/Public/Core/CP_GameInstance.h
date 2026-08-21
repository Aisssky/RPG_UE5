// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CP_GameInstance.generated.h"

UCLASS(Config = Game)
class CHAPANDA_API UCP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Cha|Multiplayer")
	void ConnectToOfficialServer();

	UPROPERTY(Config)
	FString OfficialServerIP = TEXT("127.0.0.1");

};
