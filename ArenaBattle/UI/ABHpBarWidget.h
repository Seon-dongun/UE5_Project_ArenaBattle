// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpBarWidget : public UABUserWidget
{
	GENERATED_BODY()
	
public:
	// UserWidget 클래스를 상속받는 클래스의 경우는 const FObjectInitializer& ObjectInitializer를 인자로 갖는 생성자만 있어서 반드시 형식을 맞춰주어야 함.
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override; // 게임에서 사용자 위젯이 초기화되고 준비될 때 실행

public:
	// 기존의 SetMaxHp 함수를 버리고 스탯이 업데이트 될 때마다 델리게이트로부터 이벤트를 받아서 업데이트하는 방식으로 변경
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp); // MaxHp값과 현재 HP값의 비율을 계산해서 HP바를 업데이트하는 함수
	FString GetHpStatText(); // 나중에 스탯이 업데이트 될 때, 100/100 형태로 텍스트가 바로 만들어질 수 있도록 함수를 추가

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat; // 새롭게 컨트롤을 추가했기 때문에 이것을 지원해줄 수 있는 텍스트 블록 클래스 추가

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
