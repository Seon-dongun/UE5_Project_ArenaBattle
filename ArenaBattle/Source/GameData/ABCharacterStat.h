
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase 클래스 사용을 위한 헤더
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase // 액셀 파일 import 를 위한 FTableRowBase 상속 필수
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

	// 덧셈 오버레이터에서는 모든 변수가 float으로 맞춰져있는 것을 활용하면 간단하게 두 구조체 간 덧셈을 구현할 수 있다
	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{
		// const float* ThisPtr=> 포인터가 가리키는 값을 수정할 수 없음을 나타냄
		// float* const ThisPtr => 포인터 자체를 다른 대상을 가리키도록 변경할 수 없도록 함
		// const float* const ThisPtr => 포인터가 가리키는 값을 수정할 수 없고 포인터가 가리키는 대상도 변경 불가
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FABCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FABCharacterStat) / sizeof(float); // 구조체 전체 사이즈를 해당 타입으로 나눠서 구조체 내부 요소의 수를 구한 다음
		for (int32 i = 0; i < StatNum; i++) // 반복문을 통해 각 요소들을 순서대로 더해주면, 각각의 요소를 더해주는 코드를 여러번 반복할 필요가 없다
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};