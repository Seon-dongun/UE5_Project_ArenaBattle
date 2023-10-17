#include "CharacterStat/ABCharacterStatComponent.h"
#include "GameData/ABGameSingleton.h"
UABCharacterStatComponent::UABCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.0f;

	bWantsInitializeComponent = true; // �����ڿ��� �ش簪�� true�� �����ؾ� InitializeComponent �Լ��� ȣ��ȴ�
}

void UABCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// ������ BeginPlay �Լ����� ������ ���� �ʱ�ȭ�� InitializeComponent �Լ����� �����ϵ��� ����
	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UABCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	// Clamp �Լ��� ���� ������ �������� ���� ������ ���
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UABGameSingleton::Get().CharacterMaxLevel);
	 SetBaseStat(UABGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.0f); // Ȯ���ϰ� �������� Ȯ��
}

float UABCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp(InDamage, 0, InDamage); // ���� �������� ���� ���ϰ� ����

	SetHp(FMath::Clamp(PrevHp - ActualDamage, 0.0f, BaseStat.MaxHp)); // ���� ü���� ������� �� ������ ü�� ������ ����� �ʰ� ����
	if (CurrentHp <= KINDA_SMALL_NUMBER) // KINDA_SMALL_NUMBER�� ����ϴ� ������ ���� ���� �ǹ��ϸ�, �� ������ �۴ٴ� ���� �׾��ٴ� ���� �ǹ���
	{
		OnHpZero.Broadcast(); // ���� ���, �׾��ٰ� �˸��� �ڵ����� ���� ��
	}
	return ActualDamage;
}

void UABCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp); // HP ���� �� ���� �˸��� �ڵ����� ���� ��
}