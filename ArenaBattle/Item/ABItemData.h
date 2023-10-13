#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ABItemData.generated.h"

// 아이템 종류를 지정할 수 있는 열거형
UENUM(BlueprintType) // 블루프린트와 호환될 수 있도록 BlueprintType 지정자 추가
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
		// 첫 번째 인자는 태그 이름인 ABItemdata가 되고, 두 번째 인자는 GetFName을 사용해서 애셋의 이름으로 
		// 지정하여 이 둘을 이용한 고유한 애셋 아이디를 만들 수 있다
	}
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = Type)
	EItemType Type;
	
};
