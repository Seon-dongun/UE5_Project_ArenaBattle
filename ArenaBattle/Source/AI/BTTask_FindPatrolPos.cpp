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

	APawn* ContorllingPawn = OwnerComp.GetAIOwner()->GetPawn(); // Behavior Tree를 소유한 폰 정보를 가져옴
	if (nullptr == ContorllingPawn)
	{
		return EBTNodeResult::Failed;
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ContorllingPawn->GetWorld()); // 폰이 위치한 월드값을 가져와서 해당 월드의 네비게이션 시스템을 얻는다
	if (nullptr == NavSystem)
	{
		return EBTNodeResult::Failed;
	}

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(BBKEY_HOMEPOS); // 블랙보드의 HomePos값 가져오기
	FNavLocation NextPatrolPos;

	// HomePos에서 반경 5미터에서 적당한 PatrolPos 위치를 찾아내어 NextPatrolPos에 담는다
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrolPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location); // PatrolPos값을 얻어낸 NextPatrolPos값으로 설정
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
