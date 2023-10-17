// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/ABGameSingleton.h"

DEFINE_LOG_CATEGORY(LogABGameSingleton);

UABGameSingleton::UABGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/ArenaBattle/GameData/ABCharacterStatTable.ABCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray); // 각 레벨에 대한 키, 벨류값을 가져와서
		
		// Transform 라이브러리를 통해서 벨류값에 대한 것만 뽑아내서 CharacterStatTable에 저장
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FABCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UABGameSingleton& UABGameSingleton::Get()
{
	UABGameSingleton* Singleton = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	// 우리가 설정한 싱글톤 클래스로 엔진이 알아서 싱글톤의 인스턴스를 만들어서 
	// GameSingleton이라는 변수에 저장해둔다. 싱글톤에 대한 정보를 얻을 때, 이를 가져오기만 하면 된다.
	
	if (Singleton)
	{
		return *Singleton;
	}

	// 만일 싱글톤 인스턴스를 정상적으로 가져오지 못했다면 심각한 문제이므로 에러를 발생시킨다
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UABGameSingleton>(); // 코드의 흐름을 위해 무언가 return을 반드시 해야함으로 추가한 코드. 실질적으로 사용되는 코드는 아니다
}
