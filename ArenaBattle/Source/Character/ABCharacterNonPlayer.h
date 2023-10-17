#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/ABCharacterAIInterface.h"
#include "ABCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(config=ArenaBattle) // Config�� ����Ѵٰ� �˸���, ���λ��� ArenaBattle�� ���̸� DefaultArenaBattle.ini�� �ҷ����̰ڴٴ� ���� �ǹ��Ѵ�
class ARENABATTLE_API AABCharacterNonPlayer : public AABCharacterBase, public IABCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AABCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

protected:
	void SetDead() override;
	void NPCMeshLoadCompleted(); // �ε��� �Ϸ�Ǿ��� �� ��ȣ�� �޴� �Լ�
	
	UPROPERTY(config) // ������ ���� config�� �߰��ϸ� �ش� config ���Ϸκ��� �����͸� �ҷ����ڴٴ� �ǹ�
	TArray<FSoftObjectPath> NPCMeshes; // ini�� ���� �̸��� ����Ѵ�. �̷��� ������Ʈ�� �ε��� �� �ڵ����� �� ������ ä����

	TSharedPtr<FStreamableHandle> NPCMeshHandle; // �񵿱� ������� �ε��ϱ� ���� �ڵ鷯

// AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished; // ���޵� ��������Ʈ�� �����ϴ� ����

	virtual void NotifyComboActionEnd() override; // �޺� ������ ���� �� ȣ��Ǵ� �Լ�
};
