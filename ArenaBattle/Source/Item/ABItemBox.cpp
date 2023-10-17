#include "Item/ABItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Physics/ABCollision.h"
#include "Interface/ABCharacterItemInterface.h"
#include "Engine/AssetManager.h"
#include "ABItemData.h"


AABItemBox::AABItemBox()
{
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Effect"));

	RootComponent = Trigger; // ��Ʈ ������Ʈ�� Trigger�� ����ְ�
	Mesh->SetupAttachment(Trigger); // Mesh�� SetupAttachment �Լ��� ���ؼ� ��Ʈ ������Ʈ�� Trigger�� ����
	Effect->SetupAttachment(Trigger); // Effect ���� SetupAttachment �Լ��� ���ؼ� ��Ʈ ������Ʈ�� Trigger�� ����

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER); // �ռ� ������ Ʈ���ſ� ���� �ݸ��� �����ʷ� ����
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f)); // �ڽ� ũ�� ����

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollison")); // ���ڴ� �����ֱ⸸ �ϰ� �浹�� �Ͼ�� �ʵ��� NoCollision���� ����

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object); // ����Ʈ ������ ���� �Լ��� SetTemplate �Լ�
		Effect->bAutoActivate = false; // ó���� �ٷ� �ߵ����� �ʵ��� autoactivate ������ false�� ����
	}
}


// PostInitializeComponents�� FinishSpawning �Լ� ���Ŀ� ȣ��Ǳ� ������ BeginPlay�� ������ ȿ���� ����
void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get(); // �ּ� �޴����� ������

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	// �ּ� �Ŵ����κ��� �ּµ��� �������µ�, �±� ������ �Ѱ��ָ� �츮�� ������ ���� ���� �ִ� ��� �ּ¿� ���ؼ� 
	// �̷��� �±� ���̵� ������ �ִ� �ּµ��� ����� �迭�� ��ȯ����
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UABItemData>(AssetPtr.Get()); // �ε��� ������ ������ ������ ����
	ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
	// Ʈ���� �ߵ� �� �۵��ϴ� OnComponentBeginOverlap ��������Ʈ�� ����Ʈ �ߵ��� ������ OnOverlapBegin �Լ��� ���� 
	// OnComponentBeginOverlap ��������Ʈ�� ���̳��� ��������Ʈ�̹Ƿ� AddDynamic �Լ��� ���� �����Ѵ�
}

// ��������Ʈ�� ������ ����Ʈ �ߵ��� ���� �Լ�
void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// �������� ���� ���� ���, ����Ʈ ���� ���� ����
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	// ������ ȹ�� �� ȹ���� ���Ϳ��� TakeItem �Լ��� ȣ���ϵ��� ����� �߰�
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}


	Effect->Activate(true); // ����Ʈ ��� Ȱ��ȭ
	Mesh->SetHiddenInGame(true); // ���� �޽��� ���ӿ��� ����
	SetActorEnableCollision(false); // �ٽ� �ߵ����� �ʰ� ���Ϳ� ���� �ݸ����� ��� ���ش�.

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished); // ����Ʈ ����� ������ ����� �Լ��� OnSystemFinished ��������Ʈ�� ����
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}
