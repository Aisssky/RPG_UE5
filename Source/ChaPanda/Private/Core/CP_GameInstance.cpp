// Copyright Aisssky


#include "Core/CP_GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UCP_GameInstance::ConnectToOfficialServer()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PC)) return;
	PC->ClientTravel(OfficialServerIP, TRAVEL_Absolute);
}
