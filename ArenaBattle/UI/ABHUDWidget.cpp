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

	// GetOwningPlayer �Լ��� ����ϸ� HUD�� �����ϰ� �ִ� ��Ʈ�ѷ��� ������ �� �ְ�, �ƿ� �ڵ�� ���� 
	// GetOwningPlayerPawn �Լ��� ����ϸ� HUD�� �����ϰ� �ִ� ��Ʈ�ѷ��� ������ ���� ������ �ٷ� ������ �� �ִ�
	IABCharacterHUDInterface* HUDPawn = Cast<IABCharacterHUDInterface>(GetOwningPlayerPawn()); 
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}

void UABHUDWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	FABCharacterStat TotalStat = BaseStat + ModifierStat; // ���ڷ� ���� ���̽� ����, ������̾� ������ ���� �� ���Ȱ��� ���ϰ�
	HpBar->UpdateStat(BaseStat,ModifierStat); // �� ������ MaxHp������ HP���� MaxHp���� ����

	CharacterStat->UpdateStat(BaseStat, ModifierStat); // ĳ���� ���� ������ ���Ȱ� ������Ʈ
}

void UABHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}
