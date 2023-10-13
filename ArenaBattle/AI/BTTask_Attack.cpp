
#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}

	/*
		���� ���� ���, �ٷ� ������ �׼��� �ƴϴ�.���� ��Ÿ�ְ� �������� ������ ������ ��� �� �� �ִ�.
		�׷��� ������, �츮�� ���� ����� ���ȴٰ� �ؼ� �½�ũ�� ���� ����� �����̶�� ������ �� ����.
		���� �ش� �ڵ带 ������ ���� AttackByAI �Լ��� ������ ����, ���ϰ��� ������ �� InProgress�� �������ְ�
		������ ���� ���Ŀ� Succeeded �� ��ȯ�ϸ� �ȴ�. �̸� ���ؼ��� ������ ������ Ÿ�̹��� ĳġ�ؾ��ϴµ� Tick �Լ��� �����
		��� ������ �������� �Ǵ��ϴ� ����� ������ ��������Ʈ�� Ȱ���ϸ� ȿ�������� ���� �����ϴ�.
	*/

	FAICharacterAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();
	return EBTNodeResult::InProgress;
}
