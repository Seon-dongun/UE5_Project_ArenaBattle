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
		// 첫 번째 인자는 태그 이름인 ABItemdata가 되고, 두 번째 인자는 GetFName을 사용해서 애셋의 이름으로 
		// 지정하여 이 둘을 이용한 고유한 애셋 아이디를 만들 수 있다.
		// 클래스는 다르지만 모두 다 ABItemData 라고 하는 태그를 공유하는 같은 종류의 애셋임을 지정할 수 있게 되는 것
	}

public:
	UPROPERTY(EditAnywhere,Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere,Category = Stat)
	FABCharacterStat ModifierStat;
};
