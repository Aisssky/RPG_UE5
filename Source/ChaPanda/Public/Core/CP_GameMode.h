// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameplayTagContainer.h"
#include "Character/CP_EnemyCharacter.h"
#include "CP_GameMode.generated.h"

class UCP_HeroDatalog;
UCLASS()
class CHAPANDA_API ACP_GameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ACP_GameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HeroSelect")
	TObjectPtr<UCP_HeroDatalog> HeroDatalog;

	UPROPERTY(EditDefaultsOnly,Category="Cha|Wave")
	TSubclassOf<ACP_EnemyCharacter> MeleeEnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Wave")
	TSubclassOf<ACP_EnemyCharacter> RangedEnemyClass;

	UPROPERTY(EditDefaultsOnly, Category = "Cha|Wave")
	float WaveInterval = 5.f;
	void OnEnemyKilled();
protected:
	//刷怪
	virtual void BeginPlay() override;

	void StartNextWave();
	void SpawnWave(int32 Wave);


	FTimerHandle WaveTimerHandle;
	bool bWaveInProgress = false;

	//
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
