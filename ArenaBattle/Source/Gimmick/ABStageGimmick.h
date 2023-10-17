#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate); // 스테이지 변경 시 수행되는 함수를 델리게이트로 연결
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper // 다양한 스테이지 변경에 대한 델리게이트 수행을 위해 델리게이트를 배열로 사용하기 위한 구조체 선언
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};


UENUM(BlueprintType) // 스테이지 상태를 저장하는 열거형 선언
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

UCLASS()
class ARENABATTLE_API AABStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	AABStageGimmick();

public:
	FORCEINLINE int32 GetStageNum() const { return CurrentStageNum; }
	FORCEINLINE void  SetStageNum(int32 NewStageNum) { CurrentStageNum = NewStageNum; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

// Stage Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage; // 루트 컴포넌트에 해당하는 스테이지를 담을 스태틱 메쉬 컴포넌트 변수

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger; // 플레이어가 들어왔을 때 감지할 수 있는 스테이지 트리거용 박스 컴포넌트 변수

	UFUNCTION() // 플레이어와 스테이지 트리거의 오버랩 이벤트 발동 시 받아줄 함수
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Gate Section
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates; // 4개의 문에 대한 동서남북 방향 중 하나를 키로 전달하면 바로 해당하는 문에 대한 스태틱 메쉬를 얻을 수 있도록 TMap을 사용

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers; // 4개의 문에 대한 트리거를 배열로 관리

	UFUNCTION() // 플레이어와 문 트리거의 오버랩 이벤트 발동 시 받아줄 함수
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates(); // 모든 문을 여는 함수
	void CloseAllGates(); // 모든 문을 닫는 함수

// State Section
protected:
	UPROPERTY(EditAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState);

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StateChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();

// Fight Section
protected:
	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABCharacterNonPlayer> OpponentClass; 
	// TSubclassOf 는 언리얼 엔진이 제공하는 템플릿 클래스로, 우리가 지정한 클래스로부터 상속받은 클래스 목록만
	// 표시되도록 한정해서 지정할 수 있게 기능을 제공해줌. 앞으로 구현할 NPC의 경우 블루프린트로 확장해서 
	// 다양한 캐릭터를 만들 수 있는데, 그럴 때 이것을 선택하기 위해서 모든 액터 목록을 조사하는 경우 
	// 선택할 수 있는 창이 굉장히 많이 늘어나기 때문에 클래스 정보를 한정시켜주는 TSubclassOf의 기능을 사용하면
	// 보다 편리하게 스폰시킬 NPC를 지정할 수가 있다

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime; // 스폰 딜레이 설정

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DesetroyActor);

	FTimerHandle OpponentTimerHandle;

	void OnOpponentSpawn();

// Reward Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass; // 보상 박스에 대해서 스폰할 수 있게 클래스를 지정

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;
	// RewardBoxes 변수는 우리가 스폰된 상자를 관리하기 위해서 선언한 것인데, 이 스폰된 상자들은 사실상
	// 우리가 작업하고 있는 스테이지 기믹 액터와는 무관하게 스스로 작동하는 요소이다. 이러한 요소는
	// TWeakObjectPtr 이라는 템플릿 클래스를 사용하여 약참조 방식을 사용하는 것이 좋다
	// 반면 우리가 앞에서 사용한 TObjectPtr은 강참조 방식으로써 언리얼 엔진이 해당 요소를 계속
	// 사용한다고 판단해 메모리에서 소멸시키지 않을 수 있다. 이후 완전히 액터가 소멸될 때 같이 메모리에서
	// 소멸되는 것

	TMap<FName, FVector> RewardBoxLocations; // 리워드 박스를 스폰시킬 위치에 대해서 키로 관리할 수 있도록 맵 사용

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 생성된 상자에 대해서 오버랩 이벤트가 발동될 때, 이것을 실행시키는 함수 선언

	void SpawnRewardBoxes(); // 상자 스폰 함수


// Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum; // Stage 자체 레벨값
};
