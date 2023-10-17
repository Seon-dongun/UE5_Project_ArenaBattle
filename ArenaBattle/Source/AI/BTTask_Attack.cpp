
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
		공격 같은 경우, 바로 끝나는 액션이 아니다.공격 몽타주가 끝나야지 공격이 끝났다 라고 할 수 있다.
		그렇기 때문에, 우리가 공격 명령을 내렸다고 해서 태스크의 공격 명령을 성공이라고 리턴할 수 없다.
		따라서 해당 코드를 구현할 때는 AttackByAI 함수를 실행한 다음, 리턴값을 지정할 때 InProgress로 설정해주고
		공격이 끝난 이후에 Succeeded 를 반환하면 된다. 이를 위해서는 공격이 끝나는 타이밍을 캐치해야하는데 Tick 함수를 사용해
		계속 공격이 끝났는지 판단하는 방법도 있으나 델리게이트를 활용하면 효과적으로 구현 가능하다.
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
