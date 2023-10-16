// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ABPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABPlayerController, Log, All);

/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AABPlayerController();

	// 블루프린트에서 이벤트를 발동시키는 것처럼 기능을 수행하기 위해서 BlueprintImplementableEvent 키워드를 지정한다.
	// Meta에 DisplayName을 지정하면 블루프린트에서 보여질 함수명을 지정할 수 있다
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))	
	void K2_OnScoreChanged(int32 NewScore); 
	/* 
 	   K2 라는 접두사는 블루프린트의 전신인 Kismet 이라고 하는 기능을 나타내는 관습적으로 사용하는 접두사이다. 
	   GameScoreChanged 함수와 동일한 역할 가지고 있지만 함수 이름이 달라야 해서 접두사를 사용한다.
	   K2_OnScoreChanged 의 경우는 함수를 구현하지 않아도 언리얼 엔진이 이 UFUNTION 매크로를 통해 
	   자동으로 이벤트임을 감지해서 본문을 만들어주기 때문에 오류창이 있더라도 무시해도 됨
	*/

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount); // 저장 시 발생할 이벤트 추가

	void GameScoreChanged(int32 NewScore);
	void GameClear();
	void GameOver();
	
protected:
	virtual void BeginPlay() override;	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UABHUDWidget> ABHUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UABHUDWidget> ABHUDWidget;

// Save Game Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = SaveGame)
	TObjectPtr<class UABSaveGame> SaveGameInstance;
};
