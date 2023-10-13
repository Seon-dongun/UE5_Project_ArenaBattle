// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmick/ABStageGimmick.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Physics/ABCollision.h"
#include "Character/ABCharacterNonPlayer.h"
#include "Item/ABItemBox.h"
#include "Interface/ABGameInterface.h"

AABStageGimmick::AABStageGimmick()
{
	// Stage Section
	Stage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Stage"));
	RootComponent = Stage;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StageMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Stages/SM_SQUARE.SM_SQUARE'"));
	if (StageMeshRef.Object)
	{
		Stage->SetStaticMesh(StageMeshRef.Object);
	}

	StageTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("StageTrigger"));
	StageTrigger->SetBoxExtent(FVector(775.0, 775.0f, 300.0f));
	StageTrigger->SetupAttachment(Stage);
	StageTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	StageTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnStageTriggerBeginOverlap);


	// Gate Section
	static FName GateSockets[] = { TEXT("+XGate"),TEXT("-XGATE"),TEXT("+YGate"),TEXT("-YGate") }; // �������Ͽ� �ش��ϴ� �̸��� ������ ���� �迭
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	for (FName GateSocket : GateSockets)
	{
		// �� ����
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(Stage, GateSocket); // Stage ����ƽ �޽����� ������ ���� �̸� ��ġ�� ����
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f)); // �� ��ġ �̼� ����
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // ���� ���� ������� �ʱ⿡ ����
		Gates.Add(GateSocket, Gate);

		// �� Ʈ���� ����
		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket); // �� Ʈ������ �������� ��ġ �ľ��� ���� �±� ������ ����

		GateTriggers.Add(GateTrigger); // �� Ʈ���� �迭�� ������ �� Ʈ���Ÿ� �߰�
	}

	// State Section
	CurrentState = EStageState::READY; // �ʱ� ���� ����

	// ��������Ʈ�� �Լ� ����
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));
	
	//Fight Section
	OpponentSpawnTime = 2.0f; // �⺻ ���� �ð� 2�ʷ� ����
	OpponentClass = AABCharacterNonPlayer::StaticClass();


	// Reward Section
	RewardBoxClass = AABItemBox::StaticClass(); // ���� Ŭ������ ���ؼ��� ������ �ڽ� Ŭ������ ����
	for (FName GateSocket : GateSockets) // ������ �ڽ��� ������ ��ġ�� GateSockets�� �ִ� ���� Ȱ���Ͽ� RewardBoxLocations�� �߰��Ѵ�
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}

	// Stage Stat
	CurrentStageNum = 0; // �⺻������ ���� ����
}

void AABStageGimmick::OnConstruction(const FTransform& Transform) // ��� �Ӽ��� ����� �� ȣ��ǰ� �Ǵ� �Լ�
{
	Super::OnConstruction(Transform);

	SetState(CurrentState); // CurrentState ���°��� �����ϸ� �ش� ���°����� SetState �Լ��� �����ϵ��� ��
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT); // �������� Ʈ���Ű� �ߵ��Ǹ� FIGHT ���·� ���� 
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1); // �� Ʈ���ſ� GateTrigger->ComponentTags.Add(GateSocket); �� ���� �±׸� �ϳ��� �޾Ƴ��� ������ �±� ������ 1�� �ƴϸ� �̻������� üũ
	FName ComponentTag = OverlappedComponent->ComponentTags[0]; // �� Ʈ���ſ� ����� �±� ������ ��
	FName SocketName = FName(*ComponentTag.ToString().Left(2)); // ���� 2�ܾ ©�� �����´�. �̷��� �������� -X,+X,-Y,+Y �� �ϳ��� ���� �� ����
	check(Stage->DoesSocketExist(SocketName)); // ������������ �� ���ϸ��� �ִ��� ������ Ȯ���ϰ� üũ

	FVector NewLocation = Stage->GetSocketLocation(SocketName); // �ش� ���ϸ��� ��ġ ������ ��
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);

	// �̹� �ش� ���� ��ġ�� �ٸ� ���������� ������� �������� �ֱ� ������ ���ο� ���������� ������� �� 
	// ��ġ�� ��Ұ� �ִ��� ������ Ȯ���Ѵ�
	bool bResult = GetWorld()->OverlapMultiByObjectType( 
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects, // ��� ����ƽ ������Ʈ�� ���ؼ� �˻�
		FCollisionShape::MakeSphere(775.0f), // ū ���� �ϳ� �����ؼ� �ش� ���� ��ġ���� ��ġ�� ��Ұ� �ֳ� üũ
		CollisionQueryParam // ������ this ���ڸ� ���� ��� ����ƽ ������Ʈ�� ���ؼ� �˻��� �� �ڽ��� �����ϰ� �˻���
	);

	if (!bResult) // ��ġ�� ���� �ƹ��͵� ���� ���
	{
		FTransform NewTransform(NewLocation);
		AABStageGimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AABStageGimmick>(AABStageGimmick::StaticClass(),NewTransform); // �ش� ��ġ�� ��� ���͸� ����
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1); // ���Ӱ� �����Ǵ� ���������� ���� ������������ �ϳ� ���� �������� ������ ��
			NewGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AABStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // ���� �������� ȸ��
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator); // �⺻ ȸ�������� �����Ͽ� ���� ������ ��
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	/*
		���� ���¸� ���� ���� ���¸� �����ϴ� ���� switch ���� ���ؼ��� 
		�����ϰ� ���� ���������� ���°� �������� switch�� ���� �� �ڵ尡 
		���������� ������ �ֱ� ������ ��������Ʈ�� �����ϴ� ���� ��õ�Ѵ�
	*/

	CurrentState = InNewState;
	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER); // ���������� ���� �� ���� ���·� �Ѿ �� �ֵ��� �������� Ʈ������ �ݸ����� Ȱ��ȭ
	for (auto GateTrigger : GateTriggers) // ������ ���� ���ؼ��� �ݸ����� Ȱ��ȭ�� �ʿ䰡 �����Ƿ�, ���� �ݸ����� ��Ȱ��ȭ�Ѵ�
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	OpenAllGates(); // ������ ���۵Ǳ� ������, �÷��̾ ������ ���ϵ��� ��� ���� ����
}

void AABStageGimmick::SetFight()
{
	// ���� ���� �� �������� Ʈ����, �� Ʈ���Ŵ� �ߵ��� �ʿ䰡 �����Ƿ� �ݸ����� ���� ��Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates(); // ���� Ȯ���� ���� �������� �ٽ� �� �� ��� ���� ����

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false); 
	// Ÿ�̸Ӹ� ���ؼ� ���� ������ ���� ������ ���� �ð� ���Ŀ� NPC�� ���������� �����ǵ��� �ϴ� �Լ� �ڵ� ����
}

void AABStageGimmick::SetChooseReward()
{
	// ���ڸ� �����ϱ� �������� �������� Ʈ����, �� Ʈ���Ŵ� �ߵ��� �ʿ䰡 �����Ƿ� �ݸ����� ���� ��Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates(); // ���ڸ� �����ϱ� �������� ���� ������ ���� �� �����Ƿ� ���� �����־�� ��
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	// ���� ���� �� ���� ���������� �Ѿ�� ���ؼ�, �������� Ʈ���� �ݸ����� ���ʿ������� ��Ȱ��ȭ
	// �� Ʈ���Ŵ� ���� ���������� ���� �ʿ��ϱ� ������ �� Ʈ���� �ݸ������� Ȱ��ȭ
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}
	OpenAllGates(); // ���� �������� ������ ���� ��� ���� ����
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DesetroyActor)
{
	IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
	if (ABGameMode)
	{
		ABGameMode->OnPlayerScoreChanged(CurrentStageNum); // Ŭ���� �� ���� StageNum�� ���ڷ� �־� Ȯ���Ͽ� ���������� Ŭ�����ߴ��� ���ߴ��� Ȯ���Ѵ�
		if (ABGameMode->IsGameCleared())
		{
			return; // ������ Ŭ���� �� ���¶�� ���� ���·� ������� �ʵ��� ��� ����. �̷��� ���� ���ڰ� ������ �ʰ� ���߰� �ȴ�
		}
	}
	SetState(EStageState::REWARD); // NPC ��� �� ���� ������ REWARD ���·� �̵�
}

void AABStageGimmick::OnOpponentSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f); // NPC ���� ��ġ ����
	AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (ABOpponentCharacter)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed); // NPC ��� �� �ڵ����� �����, OnDestroyed ��������Ʈ�� �Լ� ����
		
		// NPC ���� �� ����� ������ �ִ� ������ ����ؼ� NPC�� ������ �����Ѵ�
		ABOpponentCharacter->SetLevel(CurrentStageNum);
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}



void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes) // �� ���� �迭�� ��ȸ
	{
		if (RewardBox.IsValid()) // �������̹Ƿ� �̰��� ��¥ �ִ��� ������ ��ȿ���� Ȯ���� �� ���� ������ IsValid �Լ��� �迭 ��Ҹ� Ȯ���Ѵ�
		{
			AABItemBox* ValidItemBox = RewardBox.Get(); // �ִٸ� Get �Լ��� ���� �ش� ������ �ڽ��� ���� �����͸� ��´�
			AActor* OverlappedBox = OverlappedComponent->GetOwner(); // ���� ������ �̺�Ʈ�� �߻��� ����, �� �̺�Ʈ�� �߻��� ������ �ڽ� ���͸� ����
			
			// �迭�� ����� ������ �ڽ��� ������ �̺�Ʈ�� �߻��� ������ �ڽ� ���Ͱ� ���� ���� ���� ������ ������ �ڽ��̰�, 
			// �ٸ� ���� �������� ���� ������ �ڽ��̹Ƿ� �ϳ��� ���� ���� �� �ֵ��� �ϱ� ���ؼ� �������� ���� ������ ������ �ڽ����� ��� �����Ѵ�
			if (OverlappedBox != ValidItemBox) 
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT); // ������ �ڽ��� �����ߴٸ� ���� ������ NEXT ���·� �Ѿ
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f));
		AABItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AABItemBox>(RewardBoxClass, SpawnTransform);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key); // �ش� ������ �ڽ��� �±׷� ������ �ڽ��� ��ġ Ű���� �ִ´�
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap); // ������ �ڽ��� Ʈ���� �ڽ��� ������ ������ �� �� ����� ��������Ʈ�� �Լ��� ����
			RewardBoxes.Add(RewardBoxActor); // ������ �ڽ� �迭�� ������ ������ �ڽ� �߰�
		}
	}

	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
		
	}
}


