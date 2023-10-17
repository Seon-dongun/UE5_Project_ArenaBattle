// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true); // 메쉬가 로딩되기 전까지는 숨겨둔다

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0); // NPCMeshes 가 정상적으로 값이 들어왔는지 체크
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1); // NPCMeshes 중 하나를 랜덤으로 지정
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
	// 애셋 매니저를 사용해서 비동기 방식으로 AsyncLoad를 진행한다. 
	// 그리고 이 AsyncLoad가 끝나는 함수를 NPCMeshLoadCompleted 함수로 지정한다
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AABAIController* ABAIController = Cast<AABAIController>(GetController()); // 컨트롤러를 얻어와 정상적으로 AI 컨트롤러로 형변환이 되는지 체크하고
	if (ABAIController)
	{
		ABAIController->StopAI(); // 행동 트리를 멈추는 함수를 수행
	}

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AABCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid()) // 로딩이 모두 완료되었을 때, 핸들이 유효하면
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset()); // 메쉬를 가져오고
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh); // 해당 메쉬를 지정
			GetMesh()->SetHiddenInGame(false); // 메쉬가 보이지 않았다가 메쉬가 지정이 되면 보이도록 설정
		}
	}
	NPCMeshHandle->ReleaseHandle(); // 사용이 끝난 핸들 해제
}

float AABCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AABCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AABCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius()*2;
	// 공격 범위에 공격 반경까지 더한 것이 찐 공격 범위
}

float AABCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void AABCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AABCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AABCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
