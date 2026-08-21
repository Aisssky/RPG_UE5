// Copyright Aisssky


#include "UI/CP_DeathScreen.h"
#include "Components/TextBlock.h"
#include "Player/CP_PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTags/CP_Tags.h"
#include "AbilitySystem/Abilities/CP_Death.h"

void UCP_DeathScreen::StartCountdown(float Seconds)
{
	RemainingTime = Seconds;
	if (CountdownText)CountdownText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingTime)));
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UCP_DeathScreen::TickCountdown, 1.f, true);
}

void UCP_DeathScreen::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);

	if (UAbilitySystemComponent* ASC = GetOwningASC()) {
		if (const FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(UCP_Death::StaticClass())) {
			if (const UCP_Death* DeathAbility = Cast<UCP_Death>(Spec->Ability))
			{
				RespawnDelay = DeathAbility->RespawnDelay;
			}
		}
		ASC->RegisterGameplayTagEvent(CP_Tags::Status::Dead, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCP_DeathScreen::OnDeadTagChanged);

	}
}

void UCP_DeathScreen::NativeDestruct()
{
	if (UAbilitySystemComponent* ASC = GetOwningASC()) {
		ASC->RegisterGameplayTagEvent(CP_Tags::Status::Dead, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	Super::NativeDestruct();

}

UAbilitySystemComponent* UCP_DeathScreen::GetOwningASC() const
{
	if (APlayerController* PC = GetOwningPlayer()) {
		if (ACP_PlayerState* PS = PC->GetPlayerState<ACP_PlayerState>()) {
			return PS->GetAbilitySystemComponent();
		}
	}
	return nullptr;
}

void UCP_DeathScreen::TickCountdown()
{
	RemainingTime -= 1.f;
	if (CountdownText)CountdownText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingTime)));
	if(RemainingTime<=0.f)GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
}

void UCP_DeathScreen::OnDeadTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0) {
		SetVisibility(ESlateVisibility::Visible);
		StartCountdown(RespawnDelay);
	}
	else {
		SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}
