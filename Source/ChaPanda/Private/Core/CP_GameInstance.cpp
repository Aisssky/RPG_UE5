// Copyright Aisssky


#include "Core/CP_GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UCP_GameInstance::HostGame(const FString& MapName)
{
	UWorld* World = GetWorld();

	if (!IsValid(World)) return;

	UE_LOG(LogTemp, Warning, TEXT("[GameInstance] Hosting game: %s?listen"), *MapName);
	if (World->GetNetMode() == NM_Standalone) {
		UGameplayStatics::OpenLevel(World, FName(*MapName), true, TEXT("listen"));
		//还不是服务器
	}
	else {
		//已经是服务器
		World->ServerTravel(MapName + TEXT("?listen"));
	}
}

void UCP_GameInstance::JoinGame(const FString& IPAddress)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PC))
	{
		UE_LOG(LogTemp, Error, TEXT("[Multiplayer] JoinGame failed: no PlayerController"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[Multiplayer] Joining game: %s"), *IPAddress);
	PC->ClientTravel(IPAddress, TRAVEL_Absolute);
}
