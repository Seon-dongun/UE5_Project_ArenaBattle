// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/ABAIController.h"
#include "CharacterStat/ABCharacterStatComponent.h"

AABCharacterNonPlayer::AABCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true); // �޽��� �ε��Ǳ� �������� ���ܵд�

	AIControllerClass = AABAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AABCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0); // NPCMeshes �� ���������� ���� ���Դ��� üũ
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1); // NPCMeshes �� �ϳ��� �������� ����
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AABCharacterNonPlayer::NPCMeshLoadCompleted));
	// �ּ� �Ŵ����� ����ؼ� �񵿱� ������� AsyncLoad�� �����Ѵ�. 
	// �׸��� �� AsyncLoad�� ������ �Լ��� NPCMeshLoadCompleted �Լ��� �����Ѵ�
}

void AABCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	AABAIController* ABAIController = Cast<AABAIController>(GetController()); // ��Ʈ�ѷ��� ���� ���������� AI ��Ʈ�ѷ��� ����ȯ�� �Ǵ��� üũ�ϰ�
	if (ABAIController)
	{
		ABAIController->StopAI(); // �ൿ Ʈ���� ���ߴ� �Լ��� ����
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
	if (NPCMeshHandle.IsValid()) // �ε��� ��� �Ϸ�Ǿ��� ��, �ڵ��� ��ȿ�ϸ�
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset()); // �޽��� ��������
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh); // �ش� �޽��� ����
			GetMesh()->SetHiddenInGame(false); // �޽��� ������ �ʾҴٰ� �޽��� ������ �Ǹ� ���̵��� ����
		}
	}
	NPCMeshHandle->ReleaseHandle(); // ����� ���� �ڵ� ����
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
	// ���� ������ ���� �ݰ���� ���� ���� �� ���� ����
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
