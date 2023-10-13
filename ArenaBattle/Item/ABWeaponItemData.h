#pragma once

#include "CoreMinimal.h"
#include "Item/ABItemData.h"
#include "GameData/ABCharacterStat.h"
#include "ABWeaponItemData.generated.h"

UCLASS()
class ARENABATTLE_API UABWeaponItemData : public UABItemData
{
	GENERATED_BODY()

public:

	UABWeaponItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName());
		// ù ��° ���ڴ� �±� �̸��� ABItemdata�� �ǰ�, �� ��° ���ڴ� GetFName�� ����ؼ� �ּ��� �̸����� 
		// �����Ͽ� �� ���� �̿��� ������ �ּ� ���̵� ���� �� �ִ�.
		// Ŭ������ �ٸ����� ��� �� ABItemData ��� �ϴ� �±׸� �����ϴ� ���� ������ �ּ����� ������ �� �ְ� �Ǵ� ��
	}

public:
	UPROPERTY(EditAnywhere,Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere,Category = Stat)
	FABCharacterStat ModifierStat;
};
