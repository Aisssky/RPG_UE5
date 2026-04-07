// Copyright Aisssky


#include "Character/CP_BaseCharacter.h"

ACP_BaseCharacter::ACP_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

}
 