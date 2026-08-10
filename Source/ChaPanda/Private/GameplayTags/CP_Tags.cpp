// Copyright Aisssky


#include "GameplayTags/CP_Tags.h"

namespace CP_Tags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "CPTags.None", "None")

		namespace GamePhase
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Lobby, "CPTags.GamePhase.Lobby","waiting for players to join");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CharacterSelect, "CPTags.GamePhase.CharacterSelect","Hero selection phase");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(PreGame, "CPTags.GamePhase.PreGame","Pre-game countdown before match starts");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(InProgress, "CPTags.GamePhase.InProgress","Match in progress");
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(PostGame, "CPTags.GamePhase.PostGame","results screen");
	}
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

			namespace Shared
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Shared.Q", "Shared Attack Tag")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Shared.R", "Shared Attack Tag")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Shared.E", "Shared Attack Tag")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Shift, "CPTags.CPAbilities.Shared.Shift", "Shared Attack Tag")

		}
			namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "CPTags.CPAbilities.Enemy.Attack", "Enemy Attack Tag")
		}
		namespace Player
		{
			namespace Warrior
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Player.Warrior.Q", "Warrior Attack Tag")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Player.Warrior.R", "Warrior Attack Tag")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Player.Warrior.E", "Warrior Attack Tag")
			}
			namespace Mage
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Player.Mage.Q", "Mage Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Player.Mage.R", "Mage Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Player.Mage.E", "Mage Attack Tag")
			}
			namespace Archer
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Player.Archer.Q", "Archer Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Player.Archer.R", "Archer Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Player.Archer.E", "Archer Attack Tag")
			}
			namespace Doctor
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Player.Doctor.Q", "Doctor Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Player.Doctor.R", "Doctor Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Player.Doctor.E", "Doctor Attack Tag")
			}
			namespace Assassin
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q, "CPTags.CPAbilities.Player.Assassin.Q", "Assassin Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(R, "CPTags.CPAbilities.Player.Assassin.R", "Assassin Attack Tag")
					UE_DEFINE_GAMEPLAY_TAG_COMMENT(E, "CPTags.CPAbilities.Player.Assassin.E", "Assassin Attack Tag")
			}
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

		namespace Combat
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Q_Impact, "CPTags.Events.Combat.Q_Impact", "Tag for the Caombat Event")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(R_Impact, "CPTags.Events.Combat.R_Impact", "Tag for the Cambat Event")
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(E_Impact, "CPTags.Events.Combat.E_Impact", "Tag for the Cambat Event")
		}
	}
}