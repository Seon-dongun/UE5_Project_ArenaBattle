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
		DataTable->GetRowMap().GenerateValueArray(ValueArray); // �� ������ ���� Ű, �������� �����ͼ�
		
		// Transform ���̺귯���� ���ؼ� �������� ���� �͸� �̾Ƴ��� CharacterStatTable�� ����
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
	// �츮�� ������ �̱��� Ŭ������ ������ �˾Ƽ� �̱����� �ν��Ͻ��� ���� 
	// GameSingleton�̶�� ������ �����صд�. �̱��濡 ���� ������ ���� ��, �̸� �������⸸ �ϸ� �ȴ�.
	
	if (Singleton)
	{
		return *Singleton;
	}

	// ���� �̱��� �ν��Ͻ��� ���������� �������� ���ߴٸ� �ɰ��� �����̹Ƿ� ������ �߻���Ų��
	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UABGameSingleton>(); // �ڵ��� �帧�� ���� ���� return�� �ݵ�� �ؾ������� �߰��� �ڵ�. ���������� ���Ǵ� �ڵ�� �ƴϴ�
}
