#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangedDelegate); // �������� ���� �� ����Ǵ� �Լ��� ��������Ʈ�� ����
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper // �پ��� �������� ���濡 ���� ��������Ʈ ������ ���� ��������Ʈ�� �迭�� ����ϱ� ���� ����ü ����
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangedDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangedDelegate StageDelegate;
};


UENUM(BlueprintType) // �������� ���¸� �����ϴ� ������ ����
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
	TObjectPtr<class UStaticMeshComponent> Stage; // ��Ʈ ������Ʈ�� �ش��ϴ� ���������� ���� ����ƽ �޽� ������Ʈ ����

	UPROPERTY(VisibleAnywhere, Category = Stage, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger; // �÷��̾ ������ �� ������ �� �ִ� �������� Ʈ���ſ� �ڽ� ������Ʈ ����

	UFUNCTION() // �÷��̾�� �������� Ʈ������ ������ �̺�Ʈ �ߵ� �� �޾��� �Լ�
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

// Gate Section
	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates; // 4���� ���� ���� �������� ���� �� �ϳ��� Ű�� �����ϸ� �ٷ� �ش��ϴ� ���� ���� ����ƽ �޽��� ���� �� �ֵ��� TMap�� ���

	UPROPERTY(VisibleAnywhere, Category = Gate, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers; // 4���� ���� ���� Ʈ���Ÿ� �迭�� ����

	UFUNCTION() // �÷��̾�� �� Ʈ������ ������ �̺�Ʈ �ߵ� �� �޾��� �Լ�
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OpenAllGates(); // ��� ���� ���� �Լ�
	void CloseAllGates(); // ��� ���� �ݴ� �Լ�

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
	// TSubclassOf �� �𸮾� ������ �����ϴ� ���ø� Ŭ������, �츮�� ������ Ŭ�����κ��� ��ӹ��� Ŭ���� ��ϸ�
	// ǥ�õǵ��� �����ؼ� ������ �� �ְ� ����� ��������. ������ ������ NPC�� ��� �������Ʈ�� Ȯ���ؼ� 
	// �پ��� ĳ���͸� ���� �� �ִµ�, �׷� �� �̰��� �����ϱ� ���ؼ� ��� ���� ����� �����ϴ� ��� 
	// ������ �� �ִ� â�� ������ ���� �þ�� ������ Ŭ���� ������ ���������ִ� TSubclassOf�� ����� ����ϸ�
	// ���� ���ϰ� ������ų NPC�� ������ ���� �ִ�

	UPROPERTY(EditAnywhere, Category = Fight, Meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime; // ���� ������ ����

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DesetroyActor);

	FTimerHandle OpponentTimerHandle;

	void OnOpponentSpawn();

// Reward Section
protected:
	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AABItemBox> RewardBoxClass; // ���� �ڽ��� ���ؼ� ������ �� �ְ� Ŭ������ ����

	UPROPERTY(VisibleAnywhere, Category = Reward, Meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AABItemBox>> RewardBoxes;
	// RewardBoxes ������ �츮�� ������ ���ڸ� �����ϱ� ���ؼ� ������ ���ε�, �� ������ ���ڵ��� ��ǻ�
	// �츮�� �۾��ϰ� �ִ� �������� ��� ���Ϳʹ� �����ϰ� ������ �۵��ϴ� ����̴�. �̷��� ��Ҵ�
	// TWeakObjectPtr �̶�� ���ø� Ŭ������ ����Ͽ� ������ ����� ����ϴ� ���� ����
	// �ݸ� �츮�� �տ��� ����� TObjectPtr�� ������ ������ν� �𸮾� ������ �ش� ��Ҹ� ���
	// ����Ѵٰ� �Ǵ��� �޸𸮿��� �Ҹ��Ű�� ���� �� �ִ�. ���� ������ ���Ͱ� �Ҹ�� �� ���� �޸𸮿���
	// �Ҹ�Ǵ� ��

	TMap<FName, FVector> RewardBoxLocations; // ������ �ڽ��� ������ų ��ġ�� ���ؼ� Ű�� ������ �� �ֵ��� �� ���

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// ������ ���ڿ� ���ؼ� ������ �̺�Ʈ�� �ߵ��� ��, �̰��� �����Ű�� �Լ� ����

	void SpawnRewardBoxes(); // ���� ���� �Լ�


// Stage Stat
protected:
	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum; // Stage ��ü ������
};
