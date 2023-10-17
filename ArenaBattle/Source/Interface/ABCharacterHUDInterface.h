
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ABCharacterHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UABCharacterHUDInterface : public UInterface
{
	GENERATED_BODY()
};


class ARENABATTLE_API IABCharacterHUDInterface
{
	GENERATED_BODY()


public:
	virtual void SetupHUDWidget(class UABHUDWidget* InHUDWidget) = 0;
};
