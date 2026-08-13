// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CP_PlayerEntry.generated.h"

class UTextBlock;

UCLASS()
class CHAPANDA_API UCP_PlayerEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="Cha|Lobby")
	void SetPlayerName(const FString& InName);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;
};
