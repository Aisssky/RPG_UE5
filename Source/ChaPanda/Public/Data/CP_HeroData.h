// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "CP_HeroData.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UTexture2D;
class APawn;

UCLASS(BlueprintType, Blueprintable)
class CHAPANDA_API UCP_HeroData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Identity")
	FGameplayTag HeroTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Identity")
	TObjectPtr<UTexture2D> CardIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Identity")
	FText DisplayName;


	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Spawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Attributes")
	TSubclassOf<UGameplayEffect> InitAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hero|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> GrantedAbilities;


	virtual FPrimaryAssetId GetPrimaryAssetId()const override;


};
