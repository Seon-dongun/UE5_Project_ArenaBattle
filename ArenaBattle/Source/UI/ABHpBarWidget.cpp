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
	
	// 위젯에 대한 준비가 되면
	// 위젯을 소유한 액터 정보를 IABCharacterWidgetInterface 타입으로 형변환하여
	// 정상적으로 형변환 된다면 해당 캐릭터에 위젯을 세팅
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this); // 세팅할 위젯인 인자는 자기 자신을 주면 됨
	}
}

void UABHpBarWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp; // 업데이트 된 최대 체력값을 얻어오고

	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp); // 체력 바가 있다면 체력 바의 퍼센티지 설정
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}

void UABHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;

	ensure(MaxHp > 0.0f); // MaxHp가 0보다 큰 지 확실하게 체크하고 넘어감
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
		// UProgressBar 객체의 SetPercent 함수를 사용하기 위해서는 Components/ProgressBar.h 이 필요하다.
		// 해당 헤더는 UMG 모듈이 제공하고 있기 때문에 ArenaBattle.Build.cs 코드에서 모듈 이름 배열에 "UMG" 를 추가해야 함
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
