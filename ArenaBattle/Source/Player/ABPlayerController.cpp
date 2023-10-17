// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ABPlayerController.h"
#include "UI/ABHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ABSaveGame.h"

DEFINE_LOG_CATEGORY(LogABPlayerController);

AABPlayerController::AABPlayerController()
{
	static ConstructorHelpers::FClassFinder<UABHUDWidget> ABHUDWidgetRef(TEXT("/Game/ArenaBattle/UI/WBP_ABHUD.WBP_ABHUD_C")); // HUD의 클래스 정보 저장
	if (ABHUDWidgetRef.Class)
	{
		ABHUDWidgetClass = ABHUDWidgetRef.Class;
	}
}

void AABPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AABPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AABPlayerController::GameOver()
{
	K2_OnGameOver();

	// 게임이 종료되면 SaveGameToSlot 함수를 사용해 세이브 파일 명과 아이디값을 주어 게임을 저장한다
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogABPlayerController, Error, TEXT("Save Game Error!")); // 만약 저장에 실패하면 에러 로그를 띄운다
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	// 세이브 파일을 불러오는 LoadGameFromSlot 함수. TEXT 타입의 저장 파일 이름과 대응되는 
	// 플레이어의 아이디를 인자로 주면 된다. 싱글 플레이의 경우 항상 플레이어 아이디는 0이다
	SaveGameInstance = Cast<UABSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"),0)); 
	
	if (!SaveGameInstance)
	{
		SaveGameInstance = NewObject<UABSaveGame>(); // 로드 실패 시, 저장된 값이 없기 때문에 새로운 세이브 객체를 생성
		SaveGameInstance->RetryCount = 0; // 처음 세이브 생성 시 RetryCount는 0으로 초기화됨
	}
	
	SaveGameInstance->RetryCount++; // 로드 성공 시, 기존의 RetryCount를 증가
	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}
