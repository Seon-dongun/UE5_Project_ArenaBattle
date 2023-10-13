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

	// �������Ʈ���� �̺�Ʈ�� �ߵ���Ű�� ��ó�� ����� �����ϱ� ���ؼ� BlueprintImplementableEvent Ű���带 �����Ѵ�.
	// Meta�� DisplayName�� �����ϸ� �������Ʈ���� ������ �Լ����� ������ �� �ִ�
	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnScoreChangedCpp"))	
	void K2_OnScoreChanged(int32 NewScore); 
	// K2 ��� ���λ�� �������Ʈ�� ������ Kismet �̶�� �ϴ� ����� ��Ÿ���� ���������� ����ϴ� ���λ��̴�. 
	// GameScoreChanged �Լ��� ������ ���� ������ ������ �Լ� �̸��� �޶�� �ؼ� ���λ縦 ����Ѵ�.
	// K2_OnScoreChanged �� ���� �Լ��� �������� �ʾƵ� �𸮾� ������ �� UFUNTION ��ũ�θ� ���� 
	// �ڵ����� �̺�Ʈ���� �����ؼ� ������ ������ֱ� ������ ����â�� �ִ��� �����ص� ��

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameClearCpp"))
	void K2_OnGameClear();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameOverCpp"))
	void K2_OnGameOver();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, Meta = (DisplayName = "OnGameRetryCountCpp"))
	void K2_OnGameRetryCount(int32 NewRetryCount); // ���� �� �߻��� �̺�Ʈ �߰�

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
