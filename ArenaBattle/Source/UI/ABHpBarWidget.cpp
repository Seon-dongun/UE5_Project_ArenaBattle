// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/ABCharacterWidgetInterface.h"

UABHpBarWidget::UABHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1.0f;
}

void UABHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);


	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	IABCharacterWidgetInterface* CharacterWidget = Cast<IABCharacterWidgetInterface>(OwningActor);
	
	// ������ ���� �غ� �Ǹ�
	// ������ ������ ���� ������ IABCharacterWidgetInterface Ÿ������ ����ȯ�Ͽ�
	// ���������� ����ȯ �ȴٸ� �ش� ĳ���Ϳ� ������ ����
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this); // ������ ������ ���ڴ� �ڱ� �ڽ��� �ָ� ��
	}
}

void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp; // ������Ʈ �� �ִ� ü�°��� ������

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp); // ü�� �ٰ� �ִٸ� ü�� ���� �ۼ�Ƽ�� ����
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f); // MaxHp�� 0���� ū �� Ȯ���ϰ� üũ�ϰ� �Ѿ
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		// UProgressBar ��ü�� SetPercent �Լ��� ����ϱ� ���ؼ��� Components/ProgressBar.h �� �ʿ��ϴ�.
		// �ش� ����� UMG ����� �����ϰ� �ֱ� ������ ArenaBattle.Build.cs �ڵ忡�� ��� �̸� �迭�� "UMG" �� �߰��ؾ� ��
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

FString UABHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), CurrentHp,MaxHp);
}
