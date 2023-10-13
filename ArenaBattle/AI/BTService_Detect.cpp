#include "AI/BTService_Detect.h"
#include "ABAI.h"
#include "AIController.h"
#include "Interface/ABCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/ABCollision.h"
#include "DrawDebugHelpers.h"


UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect"); // ��� �̸� ����
	Interval = 1.0f; // ���͹� ���� ����
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelatSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DelatSeconds);

	APawn* ControlligPawn = OwnerComp.GetAIOwner()->GetPawn(); // AI�� ���� ���� �����ϰ� �ִ� ���� ������ ����
	if (nullptr == ControlligPawn)
	{
		return;
	}

	FVector Center = ControlligPawn->GetActorLocation(); // ���� ��ġ
	UWorld* World = ControlligPawn->GetWorld(); // ���� ���� ���尪
	if (nullptr == World)
	{
		return;
	}

	IABCharacterAIInterface* AIPawn = Cast<IABCharacterAIInterface>(ControlligPawn); // �ش� ���� �ʼ� �������̽� ��ӹ޾ҳ� üũ
	if (nullptr == AIPawn)
	{
		return;
	}

	float DetectRadius = AIPawn->GetAIDetectRange(); // �������̽��κ��� ������ ������ ���� ���� ����

	// NPC�� ���� ����
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControlligPawn);

	// �̹����� OverlapMultiByChannel �Լ��� ���� �ټ��� �����Ѵ�. ���� ������ ����� ��� ���� TArray<FOverlapResult> OverlapResults �� ���� �迭�̴�. 
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_ABACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParams
	);

	if (bResult) // NPC�� ���� ���� ��
	{
		for (auto const& OverlapResult : OverlapResults) // ������ �͵��� üũ
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController()) // �ش� ���� �����ϰ� �ִ� ���� �÷��̾� ��Ʈ�ѷ��� ���, �� �÷��̾� ĳ������ ��츸 �����Ѵ�
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn); // �������� Target���� Pawn���� ����
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f); // ���� ������ �ʷϻ����� NPC ���� ���� ǥ��

				// ���� ������ �ʷϻ����� �÷��̾���� ��ġ�� ���� ������ ǥ��
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);
				DrawDebugLine(World, ControlligPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}
	
	// ���� �÷��̾ ���ؼ� ã�� ���� ���
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr); // �������� Target���� nullptr�� ����
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f); // ���� ���� �� ���������� NPC ���� ���� ǥ��

}
