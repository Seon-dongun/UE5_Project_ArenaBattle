// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C")); // HUD�� Ŭ���� ���� ����
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	// ���ھ ����Ǿ��ٴ� GameScoreChanged �Լ��� ����Ǹ��� K2_OnScoreChanged �Լ��� ȣ��ǵ��� ������ ��´�
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AABPlayerController::GameOver()
{
	K2_OnGameOver();

	// ������ ����Ǹ� SaveGameToSlot �Լ��� ����� ���̺� ���� ��� ���̵��� �־� ������ �����Ѵ�
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!")); // ���� ���忡 �����ϸ� ���� �α׸� ����
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"),0)); 
	// ������ ���ϰ� ������ �� �ְ� ���ִ� LoadGameFromSlot �Լ�. TEXT Ÿ���� ���� ���� �̸��� �����Ǵ� 
	// �÷��̾��� ���̵� ���ڷ� �ָ� �ȴ�. �̱� �÷����� ��� �׻� �÷��̾� ���̵�� 0�̴�
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++; // �ε� ���� ��, ������ RetryCount�� ����
	}
	else
	{
		SaveGameInstance = NewObject<UABSaveGame>(); // �ε� ���� ��, ����� ���� ���� ������ ���ο� ���̺� ��ü�� ����
		SaveGameInstance->RetryCount = 0; // ó�� ���̺� ���� �� RetryCount�� 0���� �ʱ�ȭ��
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);

}
