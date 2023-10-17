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
	int32 ClearScore; // ���� �������� ������ Ŭ���� ������ �����ϴ� ����

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore; // ���� ����ǰ� �ִ� ���� ����

	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1; // ���� ������ Ŭ���� �������� �� ���ų� ������ �ٷ� ������ Ŭ������ ���¿��� �����ϰ� �ȴ�
	
};