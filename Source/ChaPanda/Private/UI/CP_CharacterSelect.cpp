// Copyright Aisssky


#include "UI/CP_CharacterSelect.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Core/CP_GameState.h"
#include "Player/CP_PlayerState.h"
#include "GameplayTags/CP_Tags.h"
#include "UI/CP_HeroCard.h"
#include "Data/CP_HeroData.h"
#include "Player/CP_PlayerController.h"

void UCP_CharacterSelect::NativeConstruct()
{
	Super::NativeConstruct();


	CardWidgets.Empty();
	if (HeroDatalog && HeroCardClass) {
		for (UCP_HeroData* HeroData : HeroDatalog->Heroes) {
			if (!HeroData)continue;
			UCP_HeroCard* Card = CreateWidget<UCP_HeroCard>(this, HeroCardClass);
			CardContainer->AddChild(Card);
			Card->SetupCard(HeroData);
			CardWidgets.Add(Card);
		}
	}
	ACP_GameState* GS =GetWorld()->GetGameState<ACP_GameState>();
	if (!GS)return;

	for (APlayerState* PS:GS->PlayerArray) {
		ACP_PlayerState* CPS = Cast<ACP_PlayerState>(PS);
		if (!IsValid(CPS)) continue;
		CPS->OnHeroSelectionChanged.AddDynamic(this, &UCP_CharacterSelect::OnHeroSelectionChangedHandler);
		CPS->OnHeroLockChanged.AddDynamic(this, &UCP_CharacterSelect::OnHeroLockChangedHandler);
	}

	RefreshAllCards();
}

void UCP_CharacterSelect::NativeDestruct()
{
	ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>();
	if (!GS)return;
	for (APlayerState* PS : GS->PlayerArray) {
		ACP_PlayerState* CPS = Cast<ACP_PlayerState>(PS);
		if (!IsValid(CPS)) continue;

		CPS->OnHeroSelectionChanged.RemoveDynamic(this, &UCP_CharacterSelect::OnHeroSelectionChangedHandler);
		CPS->OnHeroLockChanged.RemoveDynamic(this, &UCP_CharacterSelect::OnHeroLockChangedHandler);
	}
	Super::NativeDestruct();
}

void UCP_CharacterSelect::RefreshAllCards()
{
	ACP_PlayerController* PC = Cast<ACP_PlayerController>(GetOwningPlayer());
	FGameplayTag MyTag = PC ? PC->GetSelectedHeroTag() : FGameplayTag();

	bool bLocked = false;
	if (PC) {
		if (ACP_PlayerState* MyPS = PC->GetPlayerState<ACP_PlayerState>())
		{
			bLocked = MyPS->IsHeroLocked();
		}

	}
	TArray<FGameplayTag> LockedTags;
	ACP_GameState* GS = GetWorld()->GetGameState<ACP_GameState>();
	if (GS) {
		for (APlayerState* PS : GS->PlayerArray) {
			ACP_PlayerState* CPS = Cast<ACP_PlayerState>(PS);
			if (IsValid(CPS) && CPS->IsHeroLocked()) {
				LockedTags.Add(CPS->GetSelectedHeroTag());
			}
		}
	}
	//刷新卡片
	for (UCP_HeroCard* Card : CardWidgets)
	{
		FGameplayTag Tag = Card->GetHeroTag();
		Card->SetCardState(LockedTags.Contains(Tag) && MyTag != Tag, MyTag == Tag);
	}
	if (StatusText) {
		StatusText->SetText(bLocked
			? FText::FromString(TEXT("已锁定"))
			: FText::FromString(TEXT("请点击 [锁定] 确认选角")));
	}
}

void UCP_CharacterSelect::OnHeroSelectionChangedHandler(FGameplayTag OldTag, FGameplayTag NewTag)
{
	RefreshAllCards();
}

void UCP_CharacterSelect::OnHeroLockChangedHandler(bool bLocked)
{
	RefreshAllCards();
}
