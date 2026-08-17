// Copyright Aisssky


#include "Data/CP_HeroDatalog.h"
#include "Data/CP_HeroData.h"

FPrimaryAssetId UCP_HeroDatalog::GetPrimaryAssetId() const
{

	return FPrimaryAssetId(FPrimaryAssetType(TEXT("HeroDatalog")), GetFName());
}
