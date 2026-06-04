// Copyright Aisssky


#include "Character/CP_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CP_AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CP_PlayerState.h"

ACP_PlayerCharacter::ACP_PlayerCharacter()
{

	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;	
	bUseControllerRotationRoll = false;	

	GetCharacterMovement()->bOrientRotationToMovement = true;	
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

UAbilitySystemComponent* ACP_PlayerCharacter::GetAbilitySystemComponent() const
{
	ACP_PlayerState* CPPlayerState = Cast<ACP_PlayerState>(GetPlayerState());
	if(!IsValid(CPPlayerState))return nullptr;

	return CPPlayerState->GetAbilitySystemComponent();
}

UAttributeSet* ACP_PlayerCharacter::GetAttributeSet() const
{
	ACP_PlayerState* CPPlayerState = Cast<ACP_PlayerState>(GetPlayerState());
	if (!IsValid(CPPlayerState))return nullptr;

	return CPPlayerState->GetAttributeSet();
}

void ACP_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!IsValid(GetAbilitySystemComponent())|| !HasAuthority())return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());


	GiveStartupAbilities();
	InitializeAttributes();
}

void ACP_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (!IsValid(GetAbilitySystemComponent()))return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(), this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());
} 
