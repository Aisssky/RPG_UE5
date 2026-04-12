// Copyright Aisssky


#include "GameplayTags/CP_Tags.h"

namespace CP_Tags
{
	namespace CPAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG(Primary, "CP_Tags.CPAbilities.Primary");
		UE_DEFINE_GAMEPLAY_TAG(Secondary, "CP_Tags.CPAbilities.Secondary");
		UE_DEFINE_GAMEPLAY_TAG(Tertiary, "CP_Tags.CPAbilities.Tertiary");
		UE_DEFINE_GAMEPLAY_TAG(ActivateOnGiven, "CP_Tags.CPAbilities.ActivateOnGiven");
	}
	namespace Events
	{
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG(HitReact, "CP_Tags.Events.Enemy.HitReact");
		}
	}
}