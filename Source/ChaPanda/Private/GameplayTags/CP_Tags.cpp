// Copyright Aisssky


#include "GameplayTags/CP_Tags.h"

namespace CP_Tags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "CPTags.None", "None")

		namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "CPTags.SetByCaller.Projectile", "Tag for Set by Caller Magnitude for Projectiles")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "CPTags.SetByCaller.Damage", "Tag for Set by Caller Magnitude for Damage")
	}

	namespace CPAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "CPTags.CPAbilities.ActivateOnGiven", "Tag for Abilities that should activate immediately once given.")

			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "CPTags.CPAbilities.Primary", "Tag for the Primary Ability")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "CPTags.CPAbilities.Secondary", "Tag for the Secondary Ability")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "CPTags.CPAbilities.Tertiary", "Tag for the Tertiary Ability")

			namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "CPTags.CPAbilities.Enemy.Attack", "Enemy Attack Tag")
		}
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "CPTags.Events.KillScored", "Tag for the KillScored Event")

			namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "CPTags.Events.Player.HitReact", "Tag for the Player HitReact Event")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "CPTags.Events.Player.Death", "Tag for the Player Death Event")
		}

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "CPTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "CPTags.Events.Enemy.EndAttack", "Tag for the Enemy Ending an Attack")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit, "CPTags.Events.Enemy.MeleeTraceHit", "Tag for the Enemy Melee Trace Hit")
		}
	}
}