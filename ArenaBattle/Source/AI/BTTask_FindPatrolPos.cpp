// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_FindPatrolPos.h"
#include "ABAI.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ContorllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // Behavior Tree�� ������ �� ������ ������
	if (nullptr == ContorllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ContorllingPawn->GetWorld()); // ���� ��ġ�� ���尪�� �����ͼ� �ش� ������ �׺���̼� �ý����� ��´�
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS); // �������� HomePos�� ��������
	FNavLocation NextPatrolPos;

	// HomePos���� �ݰ� 5���Ϳ��� ������ PatrolPos ��ġ�� ã�Ƴ��� NextPatrolPos�� ��´�
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location); // PatrolPos���� �� NextPatrolPos������ ����
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
