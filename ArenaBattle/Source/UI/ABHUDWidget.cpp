// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHUDWidget.h"
#include "Interface/ABCharacterHUDInterface.h"
#include "ABHpBarWidget.h"
#include "ABCharacterStatWidget.h"

UABHUDWidget::UABHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UABHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UABHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UABCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	// GetOwningPlayer 함수를 사용하면 HUD를 소유하고 있는 컨트롤러를 가져올 수 있고, 아예 코드와 같이 
	// GetOwningPlayerPawn 함수를 사용하면 HUD를 소유하고 있는 컨트롤러가 방의한 폰의 정보를 바로 가져올 수 있다
	IABCharacterHUDInterface* HUDPawn = Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn()); 
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	FABCharacterStat TotalStat = BaseStat + ModifierStat; // 인자로 받은 베이스 스탯, 모디파이어 스탯을 더해 총 스탯값을 구하고
	HpBar->UpdateStat(BaseStat,ModifierStat); // 총 스탯의 MaxHp값으로 HP바의 MaxHp값을 설정

	CharacterStat->UpdateStat(BaseStat, ModifierStat); // 캐릭터 스탯 위젯의 스탯값 업데이트
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}
