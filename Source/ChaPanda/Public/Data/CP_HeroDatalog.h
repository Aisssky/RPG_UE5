// Copyright Aisssky

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CP_HeroDatalog.generated.h"

class UCP_HeroData;
UCLASS(BlueprintType, Blueprintable)
class CHAPANDA_API UCP_HeroDatalog : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Catalog")
	TArray<TObjectPtr<UCP_HeroData>> Heroes;

	virtual FPrimaryAssetId GetPrimaryAssetId()const override;

};
