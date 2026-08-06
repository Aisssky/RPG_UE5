// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CP_GameInstance.generated.h"


UCLASS()
class CHAPANDA_API UCP_GameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Cha|Multiplayer")
	void HostGame(const FString& MapName = TEXT("CPMap"));

	UFUNCTION(BlueprintCallable, Category = "Cha|Multiplayer")
	void JoinGame(const FString& IPAddress);

};
