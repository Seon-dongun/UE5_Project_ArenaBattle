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
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) = 0; // �÷��̾��� ���ھ� ���� �� ȣ��Ǵ� �Լ�
	virtual void OnPlayerDead() = 0; // �÷��̾ �׾��� �� ȣ���ϴ� �Լ�
	virtual bool IsGameCleared() = 0; // ���� ������ Ŭ���� �Ǿ����� Ȯ���ϴ� �Լ�
};
