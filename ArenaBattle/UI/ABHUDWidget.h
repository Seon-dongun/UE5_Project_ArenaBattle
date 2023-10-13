// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UABHUDWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat); // 스탯 변화에 따른 스탯 업데이트 함수
	void UpdateHpBar(float NewCurrentHp); // Hp바 업데이트 함수. ABHpBarWidget 클래스의 UpdateHpBar 함수와 동일한 함수

protected:
	UPROPERTY()
	TObjectPtr<class UABHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UABCharacterStatWidget> CharacterStat;
	
};
