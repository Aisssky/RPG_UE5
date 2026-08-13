// Copyright Aisssky


#include "UI/CP_HeroCard.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h" 
#include "Player/CP_PlayerController.h"

void UCP_HeroCard::SetupCard(FGameplayTag InHeroTag, const FText& InHeroName)
{
	HeroTag = InHeroTag;
	HeroNameText->SetText(InHeroName);
}

void UCP_HeroCard::SetCardState(bool bInIsTaken, bool bInIsMyPick)
{
	bIsTaken = bInIsTaken;
	bIsMyPick = bInIsMyPick;

	RefreshCardVisual();
}

void UCP_HeroCard::NativeConstruct()
{
	Super::NativeConstruct();
	SelectButton->OnClicked.AddDynamic(this, &UCP_HeroCard::OnSelectClicked);
	LockButton->OnClicked.AddDynamic(this, &UCP_HeroCard::OnLockClicked);
}

void UCP_HeroCard::RefreshCardVisual() {
	const FLinearColor Color = bIsMyPick ? FLinearColor(0.2f, 1.f, 0.4f, 1.f)      // 我的选择
		: bIsTaken ? FLinearColor(0.35f, 0.35f, 0.35f, 1.f)                        // 被占用
		: FLinearColor(1.f, 1.f, 1.f, 1.f);                                        // 可选
	CardBorder->SetBrushColor(Color);
	SelectButton->SetIsEnabled(!bIsTaken);   // 被占用
	LockButton->SetIsEnabled(bIsMyPick);     // 可点
}

void UCP_HeroCard::OnSelectClicked()
{
	ACP_PlayerController* PC = Cast<ACP_PlayerController>(GetOwningPlayer());
	if (PC) { PC->Server_SelectedHero(HeroTag); }

}

void UCP_HeroCard::OnLockClicked() {
	ACP_PlayerController* PC = Cast<ACP_PlayerController>(GetOwningPlayer());
	if (!PC)return;
	PC->Server_LockedInHero();

}