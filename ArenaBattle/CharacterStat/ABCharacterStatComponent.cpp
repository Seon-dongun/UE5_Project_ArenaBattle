#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true; // 생성자에서 해당값을 true로 설정해야 InitializeComponent 함수가 호출된다
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 기존에 BeginPlay 함수에서 진행한 스탯 초기화를 InitializeComponent 함수에서 수행하도록 변경
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// Clamp 함수를 통해 가능한 범위에서 현재 레벨값 얻기
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	 SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f); // 확실하게 가져온지 확인
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage); // 음의 데미지는 얻지 못하게 설정

	SetHp(FMath::Clamp(PrevHp - ActualDamage, 0.0f, BaseStat.MaxHp)); // 현재 체력을 계산했을 때 가능한 체력 범위를 벗어나지 않게 설정
	if (CurrentHp <= KINDA_SMALL_NUMBER) // KINDA_SMALL_NUMBER는 허용하는 굉장히 작은 값을 의미하며, 이 값보다 작다는 것은 죽었다는 것을 의미함
	{
		OnHpZero.Broadcast(); // 죽은 경우, 죽었다고 알림이 자동으로 가게 됨
	}
	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp); // HP 변경 시 마다 알림이 자동으로 가게 됨
}