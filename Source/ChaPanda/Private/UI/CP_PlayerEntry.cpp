// Copyright Aisssky


#include "UI/CP_PlayerEntry.h"
#include "Components/TextBlock.h"

void UCP_PlayerEntry::SetPlayerName(const FString& InName)
{
	if (PlayerNameText) {
		PlayerNameText->SetText(FText::FromString(InName));
	}
}