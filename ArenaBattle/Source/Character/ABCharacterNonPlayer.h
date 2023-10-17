#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=ArenaBattle) // Config를 사용한다고 알리고, 접두사인 ArenaBattle을 붙이면 DefaultArenaBattle.ini를 불러들이겠다는 것을 의미한다
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted(); // 로딩이 완료되었을 때 신호를 받는 함수
	
	UPROPERTY(config) // 다음과 같이 config를 추가하면 해당 config 파일로부터 데이터를 불러오겠다는 의미
	TArray<FSoftObjectPath> NPCMeshes; // ini와 같은 이름을 써야한다. 이러면 프로젝트가 로딩될 때 자동으로 이 값들이 채워짐

	TSharedPtr<FStreamableHandle> NPCMeshHandle; // 비동기 방식으로 로드하기 위한 핸들러

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished; // 전달된 델리게이트를 보관하는 변수

	virtual void NotifyComboActionEnd() override; // 콤보 공격이 끝날 때 호출되는 함수
};
