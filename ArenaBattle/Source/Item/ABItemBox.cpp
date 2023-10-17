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

	RootComponent = Trigger; // 루트 컴포넌트를 Trigger로 잡아주고
	Mesh->SetupAttachment(Trigger); // Mesh는 SetupAttachment 함수를 통해서 루트 컴포넌트인 Trigger에 부착
	Effect->SetupAttachment(Trigger); // Effect 역시 SetupAttachment 함수를 통해서 루트 컴포넌트인 Trigger에 부착

	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER); // 앞서 설정한 트리거용 전용 콜리전 프로필로 세팅
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f)); // 박스 크기 설정

	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollison")); // 상자는 보여주기만 하고 충돌은 일어나지 않도록 NoCollision으로 지정

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ArenaBattle/Effect/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (EffectRef.Object)
	{
		Effect->SetTemplate(EffectRef.Object); // 이펙트 지정을 위한 함수인 SetTemplate 함수
		Effect->bAutoActivate = false; // 처음에 바로 발동하지 않도록 autoactivate 설정을 false로 설정
	}
}


// PostInitializeComponents도 FinishSpawning 함수 이후에 호출되기 때문에 BeginPlay와 동일한 효과를 가짐
void AABItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UAssetManager& Manager = UAssetManager::Get(); // 애셋 메니저를 가져옴

	TArray<FPrimaryAssetId> Assets;
	Manager.GetPrimaryAssetIdList(TEXT("ABItemData"), Assets);
	// 애셋 매니저로부터 애셋들을 가져오는데, 태그 정보를 넘겨주면 우리가 지정한 폴더 내에 있는 모든 애셋에 대해서 
	// 이러한 태그 아이디를 가지고 있는 애셋들의 목록을 배열로 반환해줌
	ensure(0 < Assets.Num());

	int32 RandomIndex = FMath::RandRange(0, Assets.Num() - 1);
	FSoftObjectPtr AssetPtr(Manager.GetPrimaryAssetPath(Assets[RandomIndex]));
	if (AssetPtr.IsPending())
	{
		AssetPtr.LoadSynchronous();
	}
	Item = Cast<UABItemData>(AssetPtr.Get()); // 로딩된 에셋을 아이템 변수에 지정
	ensure(Item);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AABItemBox::OnOverlapBegin);
	// 트리거 발동 시 작동하는 OnComponentBeginOverlap 델리게이트에 이펙트 발동을 수행할 OnOverlapBegin 함수를 연결 
	// OnComponentBeginOverlap 델리게이트는 다이나믹 델리게이트이므로 AddDynamic 함수를 통해 연결한다
}

// 델리게이트에 연결할 이펙트 발동을 위한 함수
void AABItemBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	// 아이템이 없는 꽝의 경우, 이펙트 없이 상자 제거
	if (nullptr == Item)
	{
		Destroy();
		return;
	}

	// 아이템 획득 시 획득한 액터에게 TakeItem 함수를 호출하도록 기능을 추가
	IABCharacterItemInterface* OverlappingPawn = Cast<IABCharacterItemInterface>(OtherActor);
	if (OverlappingPawn)
	{
		OverlappingPawn->TakeItem(Item);
	}


	Effect->Activate(true); // 이펙트 재생 활성화
	Mesh->SetHiddenInGame(true); // 상자 메쉬는 게임에서 숨김
	SetActorEnableCollision(false); // 다시 발동하지 않게 액터에 대한 콜리전을 모두 없앤다.

	Effect->OnSystemFinished.AddDynamic(this, &AABItemBox::OnEffectFinished); // 이펙트 재생이 끝나면 실행될 함수를 OnSystemFinished 델리게이트에 연결
}

void AABItemBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	Destroy();
}
