#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/ABGameInterface.h"
#include "ABGameMode.generated.h"

UCLASS()
class ARENABATTLE_API AABGameMode : public AGameModeBase, public IABGameInterface
{
	GENERATED_BODY()
	
public:
	AABGameMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore; // 현재 진행중인 게임의 클리어 조건을 지정하는 변수

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore; // 현재 진행되고 있는 게임 점수

	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1; // 현재 점수가 클리어 점수보다 더 같거나 높으면 바로 게임을 클리어한 상태에서 종료하게 된다
	
};
