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
	static FName GateSockets[] = { TEXT("+XGate"),TEXT("-XGATE"),TEXT("+YGate"),TEXT("-YGate") }; // 동서남북에 해당하는 이름을 지정을 위한 배열
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GateMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_GATE.SM_GATE'"));
	for (FName GateSocket : GateSockets)
	{
		// 문 제작
		UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(GateSocket);
		Gate->SetStaticMesh(GateMeshRef.Object);
		Gate->SetupAttachment(Stage, GateSocket); // Stage 스태틱 메쉬에서 지정한 소켓 이름 위치에 부착
		Gate->SetRelativeLocation(FVector(0.0f, -80.0f, 0.0f)); // 문 위치 미세 조정
		Gate->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 문을 열린 모습으로 초기에 설정
		Gates.Add(GateSocket, Gate);

		// 문 트리거 제작
		FName TriggerName = *GateSocket.ToString().Append(TEXT("Trigger"));
		UBoxComponent* GateTrigger = CreateDefaultSubobject<UBoxComponent>(TriggerName);
		GateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
		GateTrigger->SetupAttachment(Stage, GateSocket);
		GateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
		GateTrigger->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnGateTriggerBeginOverlap);
		GateTrigger->ComponentTags.Add(GateSocket); // 문 트리거의 동서남북 위치 파악을 위한 태그 정보를 저장

		GateTriggers.Add(GateTrigger); // 문 트리거 배열에 생성한 문 트리거를 추가
	}

	// State Section
	CurrentState = EStageState::READY; // 초기 상태 설정

	// 델리게이트와 함수 연결
	StateChangeActions.Add(EStageState::READY, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetReady)));
	StateChangeActions.Add(EStageState::FIGHT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetFight)));
	StateChangeActions.Add(EStageState::REWARD, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseReward)));
	StateChangeActions.Add(EStageState::NEXT, FStageChangedDelegateWrapper(FOnStageChangedDelegate::CreateUObject(this, &AABStageGimmick::SetChooseNext)));
	
	//Fight Section
	OpponentSpawnTime = 2.0f; // 기본 스폰 시간 2초로 세팅
	OpponentClass = AABCharacterNonPlayer::StaticClass();


	// Reward Section
	RewardBoxClass = AABItemBox::StaticClass(); // 상자 클래스에 대해서는 아이템 박스 클래스로 지정
	for (FName GateSocket : GateSockets) // 리워드 박스가 스폰될 위치는 GateSockets에 있는 값을 활용하여 RewardBoxLocations에 추가한다
	{
		FVector BoxLocation = Stage->GetSocketLocation(GateSocket) / 2;
		RewardBoxLocations.Add(GateSocket, BoxLocation);
	}

	// Stage Stat
	CurrentStageNum = 0; // 기본적으로 게임 시작
}

void AABStageGimmick::OnConstruction(const FTransform& Transform) // 모든 속성이 변경될 때 호출되게 되는 함수
{
	Super::OnConstruction(Transform);

	SetState(CurrentState); // CurrentState 상태값을 변경하면 해당 상태값으로 SetState 함수를 수행하도록 함
}

void AABStageGimmick::OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetState(EStageState::FIGHT); // 스테이지 트리거가 발동되면 FIGHT 상태로 진입 
}

void AABStageGimmick::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	check(OverlappedComponent->ComponentTags.Num() == 1); // 문 트리거에 GateTrigger->ComponentTags.Add(GateSocket); 를 통해 태그를 하나씩 달아놨기 때문에 태그 갯수가 1이 아니면 이상있음을 체크
	FName ComponentTag = OverlappedComponent->ComponentTags[0]; // 문 트리거에 저장된 태그 정보를 얻어서
	FName SocketName = FName(*ComponentTag.ToString().Left(2)); // 왼쪽 2단어를 짤라서 가져온다. 이렇게 가져오면 -X,+X,-Y,+Y 중 하나를 얻어올 수 있음
	check(Stage->DoesSocketExist(SocketName)); // 스테이지에서 얻어낸 소켓명이 있는지 없는지 확실하게 체크

	FVector NewLocation = Stage->GetSocketLocation(SocketName); // 해당 소켓명의 위치 정보를 얻어냄
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(GateTrigger), false, this);

	// 이미 해당 소켓 위치에 다른 스테이지가 만들어져 있을수도 있기 때문에 새로운 스테이지를 만들었을 때 
	// 겹치는 요소가 있는지 없는지 확인한다
	bool bResult = GetWorld()->OverlapMultiByObjectType( 
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams::InitType::AllStaticObjects, // 모든 스태틱 오브젝트에 대해서 검사
		FCollisionShape::MakeSphere(775.0f), // 큰 원을 하나 생성해서 해당 소켓 위치에서 겹치는 요소가 있나 체크
		CollisionQueryParam // 마지막 this 인자를 통해 모든 스태틱 오브젝트에 대해서 검사할 때 자신을 제외하고 검사함
	);

	if (!bResult) // 겹치는 것이 아무것도 없는 경우
	{
		FTransform NewTransform(NewLocation);
		AABStageGimmick* NewGimmick = GetWorld()->SpawnActorDeferred<AABStageGimmick>(AABStageGimmick::StaticClass(),NewTransform); // 해당 위치에 기믹 액터를 스폰
		if (NewGimmick)
		{
			NewGimmick->SetStageNum(CurrentStageNum + 1); // 새롭게 스폰되는 스테이지는 이전 스테이지보다 하나 높은 레벨값을 갖도록 함
			NewGimmick->FinishSpawning(NewTransform);
		}
	}
}

void AABStageGimmick::OpenAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // 문이 열리도록 회전
	}
}

void AABStageGimmick::CloseAllGates()
{
	for (auto Gate : Gates)
	{
		(Gate.Value)->SetRelativeRotation(FRotator::ZeroRotator); // 기본 회전값으로 설정하여 문이 닫히게 함
	}
}

void AABStageGimmick::SetState(EStageState InNewState)
{
	/*
		현재 상태를 보고 다음 상태를 설정하는 것을 switch 문을 통해서도 
		간단하게 구현 가능하지만 상태가 많아지면 switch로 구현 시 코드가 
		복잡해지는 문제가 있기 때문에 델리게이트로 구현하는 것을 추천한다
	*/

	CurrentState = InNewState;
	if (StateChangeActions.Contains(InNewState))
	{
		StateChangeActions[CurrentState].StageDelegate.ExecuteIfBound();
	}
}

void AABStageGimmick::SetReady()
{
	StageTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER); // 스테이지에 진입 시 다음 상태로 넘어갈 수 있도록 스테이지 트리거의 콜리전을 활성화
	for (auto GateTrigger : GateTriggers) // 각각의 문에 대해서는 콜리전을 활성화할 필요가 없으므로, 전부 콜리전을 비활성화한다
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	OpenAllGates(); // 대전이 시작되기 때문에, 플레이어가 나가지 못하도록 모든 문을 닫음
}

void AABStageGimmick::SetFight()
{
	// 대전 시작 시 스테이지 트리거, 문 트리거는 발동할 필요가 없으므로 콜리전을 전부 비활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates(); // 문이 확실히 전부 닫히도록 다시 한 번 모든 문을 닫음

	GetWorld()->GetTimerManager().SetTimer(OpponentTimerHandle, this, &AABStageGimmick::OnOpponentSpawn, OpponentSpawnTime, false); 
	// 타이머를 통해서 문이 닫히고 나서 지정된 스폰 시간 이후에 NPC가 스테이지에 생성되도록 하는 함수 자동 실행
}

void AABStageGimmick::SetChooseReward()
{
	// 상자를 선택하기 전까지는 스테이지 트리거, 문 트리거는 발동할 필요가 없으므로 콜리전을 전부 비활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	}
	CloseAllGates(); // 상자를 선택하기 전까지는 다음 맵으로 나갈 수 없으므로 문이 닫혀있어야 함
	SpawnRewardBoxes();
}

void AABStageGimmick::SetChooseNext()
{
	// 상자 선택 후 다음 스테이지로 넘어가기 위해서, 스테이지 트리거 콜리전은 불필요함으로 비활성화
	// 문 트리거는 다음 스테이지를 위해 필요하기 때문에 문 트리거 콜리전만을 활성화
	StageTrigger->SetCollisionProfileName(TEXT("NoCollision"));
	for (auto GateTrigger : GateTriggers)
	{
		GateTrigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	}
	OpenAllGates(); // 다음 스테이지 진입을 위해 모든 문을 연다
}

void AABStageGimmick::OnOpponentDestroyed(AActor* DesetroyActor)
{
	IABGameInterface* ABGameMode = Cast<IABGameInterface>(GetWorld()->GetAuthGameMode());
	if (ABGameMode)
	{
		ABGameMode->OnPlayerScoreChanged(CurrentStageNum); // 클리어 한 현재 StageNum을 인자로 주어 확인하여 게임 클리어 여부를 확인한다
		if (ABGameMode->IsGameCleared())
		{
			return; // 게임이 클리어 된 상태라면 다음 상태로 진행되지 않도록 즉시 리턴. 이러면 보상 상자가 나오지 않고 멈추게 된다
		}
	}
	SetState(EStageState::REWARD); // NPC 사망 시 다음 상태인 REWARD 상태로 이동
}

void AABStageGimmick::OnOpponentSpawn()
{
	const FTransform SpawnTransform(GetActorLocation() + FVector::UpVector * 88.0f); // NPC 스폰 위치 설정
	AABCharacterNonPlayer* ABOpponentCharacter = GetWorld()->SpawnActorDeferred<AABCharacterNonPlayer>(OpponentClass, SpawnTransform);
	if (ABOpponentCharacter)
	{
		ABOpponentCharacter->OnDestroyed.AddDynamic(this, &AABStageGimmick::OnOpponentDestroyed); // NPC 사망 시 자동으로 실행될, OnDestroyed 델리게이트에 함수 연결
		
		// NPC 스폰 시 기믹이 가지고 있는 정보를 사용해서 NPC의 레벨을 지정한다
		ABOpponentCharacter->SetLevel(CurrentStageNum);
		ABOpponentCharacter->FinishSpawning(SpawnTransform);
	}
}



void AABStageGimmick::OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (const auto& RewardBox : RewardBoxes) // 약 참조 배열을 순회
	{
		if (RewardBox.IsValid()) // 약참조이므로 이것이 진짜 있는지 없는지 유효성을 확신할 수 없기 때문에 IsValid 함수로 배열 요소를 확인한다
		{
			AABItemBox* ValidItemBox = RewardBox.Get(); // 있다면 Get 함수를 통해 해당 리워드 박스에 대한 포인터를 얻는다
			AActor* OverlappedBox = OverlappedComponent->GetOwner(); // 현재 오버랩 이벤트가 발생한 액터, 즉 이벤트가 발생한 리워드 박스 액터를 얻어옴
			
			// 배열에 저장된 리워드 박스와 오버랩 이벤트가 발생한 리워드 박스 액터가 서로 같은 것은 습득한 리워드 박스이고, 
			// 다른 것은 습득하지 않은 리워드 박스이므로 하나의 보상만 먹을 수 있도록 하기 위해서 습득하지 않은 나머지 리워드 박스들은 즉시 제거한다
			if (OverlappedBox != ValidItemBox) 
			{
				ValidItemBox->Destroy();
			}
		}
	}

	SetState(EStageState::NEXT); // 리워드 박스를 습득했다면 다음 상태인 NEXT 상태로 넘어감
}

void AABStageGimmick::SpawnRewardBoxes()
{
	for (const auto& RewardBoxLocation : RewardBoxLocations)
	{
		FTransform SpawnTransform(GetActorLocation() + RewardBoxLocation.Value + FVector(0.0f, 0.0f, 30.0f));
		AABItemBox* RewardBoxActor = GetWorld()->SpawnActorDeferred<AABItemBox>(RewardBoxClass, SpawnTransform);
		if (RewardBoxActor)
		{
			RewardBoxActor->Tags.Add(RewardBoxLocation.Key); // 해당 리워드 박스의 태그로 리워드 박스의 위치 키값을 넣는다
			RewardBoxActor->GetTrigger()->OnComponentBeginOverlap.AddDynamic(this, &AABStageGimmick::OnRewardTriggerBeginOverlap); // 리워드 박스의 트리거 박스를 가져와 오버랩 될 때 수행될 델리게이트와 함수를 연결
			RewardBoxes.Add(RewardBoxActor); // 리워드 박스 배열에 스폰된 리워드 박스 추가
		}
	}
	
	// 지연을 통해 앞에서 완전히 상자에 대한 설정을 끝나고 지연을 해제해
	// ABItemBox의 PostInitializeComponents 함수를 나중에 진행시킴
	for (const auto& RewardBox : RewardBoxes)
	{
		if (RewardBox.IsValid())
		{
			RewardBox.Get()->FinishSpawning(RewardBox.Get()->GetActorTransform());
		}
		
	}
}


