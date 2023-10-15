#include "AI/BTService_Detect.h"
#include "ABAI.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); // 노드 이름 지정
	Interval = 1.0f; // 인터벌 간격 설정
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelatSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelatSeconds);

	APawn* ControlligPawn = OwnerComp.GetAIOwner()->GetPawn(); // AI로 부터 지금 제어하고 있는 폰의 정보를 얻어옴
	if (nullptr == ControlligPawn)
	{
		return;
	}

	FVector Center = ControlligPawn->GetActorLocation(); // 폰의 위치
	UWorld* World = ControlligPawn->GetWorld(); // 폰이 속한 월드값
	if (nullptr == World)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControlligPawn); // 해당 폰이 필수 인터페이스 상속받았나 체크
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); // 인터페이스로부터 감지할 영역에 대한 값을 얻어옴

	// NPC의 감지 구현
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControlligPawn);

	// 이번에는 OverlapMultiByChannel 함수를 통해 다수를 감지한다. 따라서 감지된 결과를 담는 것이 TArray<FOverlapResult> OverlapResults 와 같은 배열이다. 
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult) // NPC가 감지 성공 시
	{
		for (auto const& OverlapResult : OverlapResults) // 감지한 것들을 체크
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) // 해당 폰을 조종하고 있는 것이 플레이어 컨트롤러인 경우, 즉 플레이어 캐릭터인 경우만 감지한다
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn); // 블랙보드의 Target값을 Pawn으로 지정
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // 감지 성공시 초록색으로 NPC 감지 영역 표현

				// 감지 성공시 초록색으로 감지 영역 표현
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControlligPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}
	
	// 만약 플레이어에 대해서 찾지 못한 경우
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr); // 블랙보드의 Target값을 nullptr로 지정
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); // 감지 실패 시 붉은색으로 NPC 감지 영역 표현

}
