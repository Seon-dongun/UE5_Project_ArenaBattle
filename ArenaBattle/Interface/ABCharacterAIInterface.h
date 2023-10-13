
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterAIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UABCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};


DECLARE_DELEGATE(FAICharacterAttackFinished);


class ARENABATTLE_API IABCharacterAIInterface
{
	GENERATED_BODY()

// NPC가 의무적으로 가져야 될 기본적인 데이터를 얻을 수 있는 함수
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0; // 공격 명령을 위한 함수
};
