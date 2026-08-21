// Copyright Aisssky


#include "Player/CP_PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/CP_BaseCharacter.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CP_Tags.h"
#include "Player/CP_PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Core/CP_LobbyGameMode.h"
#include "Core/CP_GameState.h"

void ACP_PlayerController::OnGamePhaseChanged(FGameplayTag OldPhase, FGameplayTag NewPhase)
{
	if (!IsLocalPlayerController()) return;

	if (NewPhase == CP_Tags::GamePhase::Lobby || NewPhase == CP_Tags::GamePhase::CharacterSelect)
	{
		SetIgnoreLookInput(true);
		SetShowMouseCursor(true);
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
		if (!LobbyWidgetInstance && LobbyWidgetClass)
		{
			LobbyWidgetInstance = CreateWidget<UUserWidget>(this, LobbyWidgetClass);
			if (LobbyWidgetInstance)
			{
				LobbyWidgetInstance->AddToViewport();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[UIDebug] CreateWidget returned NULL, class=%s"), *LobbyWidgetClass->GetName());
			}
		}
	}
	else if (NewPhase == CP_Tags::GamePhase::InProgress)
	{
		SetIgnoreLookInput(false);
		SetShowMouseCursor(false);
		SetInputMode(FInputModeGameOnly());
	}
}

void ACP_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("[UIDebug] OnPossess: HasAuthority=%d IsLocal=%d Pawn=%s"),
		(int32)HasAuthority(), (int32)IsLocalPlayerController(),
		InPawn ? *InPawn->GetName() : TEXT("NULL"));
	if (!IsLocalPlayerController()) return;

	SetIgnoreLookInput(false);
	SetShowMouseCursor(false);
	SetInputMode(FInputModeGameOnly());
	if (LobbyWidgetInstance)
	{
		LobbyWidgetInstance->RemoveFromParent();
		LobbyWidgetInstance = nullptr;
	}
}

void ACP_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>();
	UE_LOG(LogTemp, Warning, TEXT("[UIDebug] PC BeginPlay: HasAuthority=%d GS=%s Phase=%s"),
		(int32)HasAuthority(),
		GS ? TEXT("Valid") : TEXT("NULL"),
		GS ? *GS->GamePhaseTag.ToString() : TEXT("NULL"));
	if (GS)
	{
		GS->OnGamePhaseChanged.AddDynamic(this, &ACP_PlayerController::OnGamePhaseChanged);
		OnGamePhaseChanged(FGameplayTag(), GS->GamePhaseTag);
	}
}

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
	EnhancedInputComponent->BindAction(EAction, ETriggerEvent::Completed, this, &ACP_PlayerController::EAbilityReleased);
}
void ACP_PlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACP_PlayerController::Server_SelectedHero_Implementation(const FGameplayTag& HeroTag)
{
	ACP_PlayerState* PS = GetPlayerState<ACP_PlayerState>();
	if (!IsValid(PS))return;
	PS->Server_SelectHero(HeroTag);
}

void ACP_PlayerController::Server_LockedInHero_Implementation()
{
	ACP_PlayerState* PS = GetPlayerState<ACP_PlayerState>();
	if (!IsValid(PS))return;
	PS->Server_LockInHero();
}

void ACP_PlayerController::Server_StartGame_Implementation(const FString& MapName)
{
	ACP_LobbyGameMode* GM = GetWorld()->GetAuthGameMode<ACP_LobbyGameMode>();

	if (!IsValid(GM))return;
	GM->StartGame(MapName);
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
	ActivateAbility(CP_Tags::CPAbilities::Shared::R);
}

void ACP_PlayerController::EAbility()
{
	if (!IsAlive()) return;

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC)) return;

	// LocalPredicted
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.Ability->AbilityTags.HasTag(CP_Tags::CPAbilities::Shared::E))
		{
			ASC->AbilitySpecInputPressed(const_cast<FGameplayAbilitySpec&>(Spec));
			ASC->TryActivateAbility(Spec.Handle);
			break;
		}
	}
}

void ACP_PlayerController::EAbilityReleased()
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn());
	if (!IsValid(ASC)) return;

	// LocalPredicted
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.IsActive() && Spec.Ability->AbilityTags.HasTag(CP_Tags::CPAbilities::Shared::E))
		{
			ASC->AbilitySpecInputReleased(const_cast<FGameplayAbilitySpec&>(Spec));
			break;
		}
	}
}



bool ACP_PlayerController::IsAlive() const
{
	ACP_BaseCharacter* BaseCharacter = Cast<ACP_BaseCharacter>(GetPawn());
	if (!IsValid(BaseCharacter))return false;
	return BaseCharacter->IsAlive();
}



FGameplayTag ACP_PlayerController::GetSelectedHeroTag() const
{
	if (const ACP_PlayerState* PS = GetPlayerState<ACP_PlayerState>()) {
		return PS->GetSelectedHeroTag();
	}
	return FGameplayTag();
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