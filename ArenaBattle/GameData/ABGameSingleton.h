// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABCharacterStat.h"
#include "ABGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABGameSingleton, Error, All); // 로그 정의

UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UABGameSingleton();
	static UABGameSingleton& Get(); // 싱글톤 레퍼런스를 가져오는 Get 함수

// Character Stat Data Section
public:
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel-1) ? CharacterStatTable[InLevel-1] : FABCharacterStat(); }
	// 레벨 정보를 주면, 테이블에서 레벨에 해당하는 하나의 구조체 정보를 리턴하는 함수
	// 해당 레벨값이 유효하다면 그 레벨에 대한 정보를, 없다면 기본 생성자를 통한 기본 정보 제공

	UPROPERTY()
	int32 CharacterMaxLevel; // 총 레벨 갯수를 담는 변수

private:
	TArray<FABCharacterStat> CharacterStatTable; // 캐릭터 스탯들에 대한 테이블들을 보관하는 배열
};
