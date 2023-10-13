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
	// UserWidget Ŭ������ ��ӹ޴� Ŭ������ ���� const FObjectInitializer& ObjectInitializer�� ���ڷ� ���� �����ڸ� �־ �ݵ�� ������ �����־�� ��.
	UABHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override; // ���ӿ��� ����� ������ �ʱ�ȭ�ǰ� �غ�� �� ����

public:
	// ������ SetMaxHp �Լ��� ������ ������ ������Ʈ �� ������ ��������Ʈ�κ��� �̺�Ʈ�� �޾Ƽ� ������Ʈ�ϴ� ������� ����
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp); // MaxHp���� ���� HP���� ������ ����ؼ� HP�ٸ� ������Ʈ�ϴ� �Լ�
	FString GetHpStatText(); // ���߿� ������ ������Ʈ �� ��, 100/100 ���·� �ؽ�Ʈ�� �ٷ� ������� �� �ֵ��� �Լ��� �߰�

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat; // ���Ӱ� ��Ʈ���� �߰��߱� ������ �̰��� �������� �� �ִ� �ؽ�Ʈ ��� Ŭ���� �߰�

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
