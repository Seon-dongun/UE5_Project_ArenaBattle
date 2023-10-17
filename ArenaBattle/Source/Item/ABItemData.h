#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// ������ ������ ������ �� �ִ� ������
UENUM(BlueprintType) // �������Ʈ�� ȣȯ�� �� �ֵ��� BlueprintType ������ �߰�
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

UCLASS()
class ARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName()); 
		// ù ��° ���ڴ� �±� �̸��� ABItemdata�� �ǰ�, �� ��° ���ڴ� GetFName�� ����ؼ� �ּ��� �̸����� 
		// �����Ͽ� �� ���� �̿��� ������ �ּ� ���̵� ���� �� �ִ�
	}
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Type)
	EItemType Type;
	
};
