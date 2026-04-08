// Copyright Aisssky


#include "Player/CP_PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputTriggers.h"
#include "GameFramework/Character.h"

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

}
void ACP_PlayerController::Jump()
{
	if(!IsValid(GetCharacter()))return;
	GetCharacter()->Jump();
}

void ACP_PlayerController::StopJumping()
{
	if (!IsValid(GetCharacter()))return;
	GetCharacter()->StopJumping();
}

void ACP_PlayerController::Look(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))return;
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	
	AddYawInput(LookAxisValue.X);
	AddPitchInput(LookAxisValue.Y);

}

void ACP_PlayerController::Move(const FInputActionValue& Value)
{
	if (!IsValid(GetPawn()))return;
	const FVector2D MovementVector = Value.Get<FVector2D>(); 

	const FRotator YawRotation(0.f,GetControlRotation().Yaw,0.f);
	const FVector ForwardDireaction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetPawn()->AddMovementInput(ForwardDireaction, MovementVector.Y);	
	GetPawn()->AddMovementInput(RightDirection, MovementVector.X);

}
void ACP_PlayerController::PrimaryAttack()
{
	UE_LOG(LogTemp, Warning, TEXT("Primary Attack"));
}