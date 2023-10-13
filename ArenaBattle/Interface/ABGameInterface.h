#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABGameInterface.generated.h"

UINTERFACE(MinimalAPI)
class UABGameInterface : public UInterface
{
	GENERATED_BODY()
};


class ARENABATTLE_API IABGameInterface
{
	GENERATED_BODY()

public:
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) = 0; // 플레이어의 스코어 변경 시 호출되는 함수
	virtual void OnPlayerDead() = 0; // 플레이어가 죽었을 때 호출하는 함수
	virtual bool IsGameCleared() = 0; // 현재 게임이 클리어 되었는지 확인하는 함수
};
