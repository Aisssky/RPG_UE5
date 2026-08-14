// Copyright Aisssky


#include "Data/CP_HeroData.h"

FPrimaryAssetId UCP_HeroData::GetPrimaryAssetId() const
{

	return FPrimaryAssetId(FPrimaryAssetType(TEXT("HeroData")), GetFName());
}
