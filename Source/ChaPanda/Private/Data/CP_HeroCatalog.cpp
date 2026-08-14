// Copyright Aisssky


#include "Data/CP_HeroCatalog.h"
#include "Data/CP_HeroData.h"

FPrimaryAssetId UCP_HeroCatalog::GetPrimaryAssetId() const
{

	return FPrimaryAssetId(FPrimaryAssetType(TEXT("HeroCatalog")), GetFName());
}
