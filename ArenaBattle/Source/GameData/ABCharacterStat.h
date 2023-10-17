
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase Ŭ���� ����� ���� ���
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase // �׼� ���� import �� ���� FTableRowBase ��� �ʼ�
{
	GENERATED_BODY()

public:
	FABCharacterStat() : MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f),MovementSpeed(0.0f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MovementSpeed;

	// ���� ���������Ϳ����� ��� ������ float���� �������ִ� ���� Ȱ���ϸ� �����ϰ� �� ����ü �� ������ ������ �� �ִ�
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// const float* ThisPtr=> �����Ͱ� ����Ű�� ���� ������ �� ������ ��Ÿ��
		// float* const ThisPtr => ������ ��ü�� �ٸ� ����� ����Ű���� ������ �� ������ ��
		// const float* const ThisPtr => �����Ͱ� ����Ű�� ���� ������ �� ���� �����Ͱ� ����Ű�� ��� ���� �Ұ�
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FABCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float); // ����ü ��ü ����� �ش� Ÿ������ ������ ����ü ���� ����� ���� ���� ����
		for (int32 i = 0; i < StatNum; i++) // �ݺ����� ���� �� ��ҵ��� ������� �����ָ�, ������ ��Ҹ� �����ִ� �ڵ带 ������ �ݺ��� �ʿ䰡 ����
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};