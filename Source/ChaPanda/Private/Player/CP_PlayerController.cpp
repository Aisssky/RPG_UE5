// Copyright Aisssky


#include "Player/CP_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_BaseCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CP_Tags.h"

#include "Net/UnrealNetwork.h"



void ACP_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if(!IsValid(InputSubsystem))return;

	for (UInputMappingContext* Context : InputMappingContexts)
	{
		InputSubsystem->AddMappingContext(Context, 0);
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!IsValid(EnhancedInputComponent))return;

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACP_PlayerController::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACP_PlayerController::StopJumping);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACP_PlayerController::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACP_PlayerController::Move);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ACP_PlayerController::PrimaryAttack);
	EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &ACP_PlayerController::SecondaryAttack);
	EnhancedInputComponent->BindAction(TertiaryAction, ETriggerEvent::Started, this, &ACP_PlayerController::TertiaryAttack);
	EnhancedInputComponent->BindAction(QAction, ETriggerEvent::Started, this, &ACP_PlayerController::QAbility);
	EnhancedInputComponent->BindAction(RAction, ETriggerEvent::Started, this, &ACP_PlayerController::RAbility);
	EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Started, this, &ACP_PlayerController::EAbility);

}
void ACP_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACP_PlayerController, SelectedHeroTag);
}
void ACP_PlayerController::Server_SetSelectedHeroTag_Implementation(const FGameplayTag& HeroTag)
{
	if (!HeroTag.IsValid())return;
	SelectedHeroTag = HeroTag;
}
void ACP_PlayerController::Jump()
{
	if(!IsValid(GetCharacter()))return;
	if (!IsAlive())return;

	GetCharacter()->Jump();
}

void ACP_PlayerController::StopJumping()
{
	if (!IsValid(GetCharacter()))return;
	if (!IsAlive())return;

	GetCharacter()->StopJumping();
}

void ACP_PlayerController::Look(const FInputActionValue& Value)
{
	if (!IsAlive())return;
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	
	AddYawInput(LookAxisValue.X);
	AddPitchInput(LookAxisValue.Y);

}

void ACP_PlayerController::Move(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))return;
	if (!IsAlive())return;

	const FVector2D MovementVector = Value.Get<FVector2D>(); 

	const FRotator YawRotation(0.f,GetControlRotation().Yaw,0.f);
	const FVector ForwardDireaction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDireaction, MovementVector.Y);	
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);

}
void ACP_PlayerController::PrimaryAttack()
{
	ActivateAbility(CP_Tags::CPAbilities::Primary);
}

void ACP_PlayerController::ActivateAbility(const FGameplayTag& AbilityTag) const
{
	if (!IsAlive())return;

	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		const_cast<ACP_PlayerController*>(this)->Server_ActivateAbility(AbilityTag);
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC))return;

	ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());

}

void ACP_PlayerController::SecondaryAttack()
{
	ActivateAbility(CP_Tags::CPAbilities::Secondary);
}

void ACP_PlayerController::TertiaryAttack()
{
	ActivateAbility(CP_Tags::CPAbilities::Tertiary);
}

void ACP_PlayerController::QAbility()
{
	ActivateAbility(CP_Tags::CPAbilities::Shared::Q);
}

void ACP_PlayerController::RAbility()
{
}

void ACP_PlayerController::EAbility()
{
}

bool ACP_PlayerController::IsAlive() const
{
	ACP_BaseCharacter* BaseCharacter = Cast<ACP_BaseCharacter>(GetPawn());
	if (!IsValid(BaseCharacter))return false;
	return BaseCharacter->IsAlive();
}

FGameplayTag ACP_PlayerController::GetSelectedHeroTag() const
{
	return SelectedHeroTag;
}

void ACP_PlayerController::Client_NotifyAbilityRejected_Implementation(const FGameplayTag& AbilityTag)
{
	UE_LOG(LogTemp, Warning, TEXT("[RPC] Ability rejected by server: %s"), *AbilityTag.ToString());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red,
			FString::Printf(TEXT("Server rejected: %s"), *AbilityTag.ToString()));
	}
}

void ACP_PlayerController::Server_ActivateAbility_Implementation(const FGameplayTag& AbilityTag)
{
	UE_LOG(LogTemp, Warning, TEXT("[RPC] Server received ability request: %s"), *AbilityTag.ToString());

	if (!AbilityTag.IsValid())return;
	if (!IsAlive())return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC)) return;

	bool bSuccess = ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer());

	// 如果失败，通知这个客户端
	if (!bSuccess)
	{
		Client_NotifyAbilityRejected(AbilityTag);
	}
}

bool ACP_PlayerController::Server_ActivateAbility_Validate(const FGameplayTag& AbilityTag)
{
	if(!AbilityTag.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("[RPC Validate] Invalid tag! Possible cheat attempt."));
		return false;
	}
	return true;
}