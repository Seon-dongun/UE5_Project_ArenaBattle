#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate); // HP�� 0�� �� �׾��ٶ�� �ñ׳��� ������ ��������Ʈ
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/); // ����� ���� HP ���� ������ ��ü�鿡�� ������ ��������Ʈ
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat& /*BaseStat*/, const FABCharacterStat& /*ModifierStat*/);// ������ ������ ������ �˸��� ������ ��������Ʈ

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARENABATTLE_API UABCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UABCharacterStatComponent();

protected:
	virtual void InitializeComponent() override;

public:
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// �ִ��� ���ϸ� ���̱� ���� Tick �Լ��� ȣ������ �ʰ� �ʿ��� ���� ȣ��ǵ��� �� ���̴�

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel); // ���� ���� Set
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; } // ���� ���� Get
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat) { BaseStat = BaseStat+InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());} // �߰� ������ �����ְ� ���� ��ȭ�� ���� ��������Ʈ �˸� ������
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(),GetModifierStat()); } //Base Stat Set
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(),GetModifierStat()); } //Modifier Stat Set
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; } // ���� �� �� Get
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; };
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); } // �ִ� ü�°������� ȸ���� �� �ֵ��� ���� �����ϰ� ü���� ��ȭ������ ü�� ��ȭ�� ���� ��������Ʈ �˸� ������
	float ApplyDamage(float InDamage); // �������� �޾��� �� �������� �����ϴ� �Լ�

	// ���̽� ����, ������̾� ���� Getter �Լ� �߰�
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }


protected:
	// HP ���� �����Ǿ��� �� ����� �Լ�. HP ���� �� �ݵ�� �� �Լ��� ȣ���ϵ��� ����
	void SetHp(float NewHp);

	/* 
		���� �ڵ忡 �ִ� MaxHp ������ �⺻ ���ݿ� �ִ� �����̹Ƿ� ����
	*/

	// �Ʒ� 4���� ���� ������ ������ ���Ӱ� �����Ǳ� ������ ��ũ�� ������ �ʿ䰡 ���� �� �ִ�. �̿� ���� ��ũ�� ������ �ʿ䰡 ���ٸ�
	// Transient Ű���带 �߰��Ͽ� ��ũ�κ��� ������ �� ���ʿ��� ������ �������� �ʵ��� �������� �� ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient,VisibleInstanceOnly,Category = Stat)
	float CurrentLevel; // ���� ������ ������� ���� �̱������κ��� BaseStat ������ �����ޱ� ���� �ʿ��� ���� ������ ���� ����

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FABCharacterStat BaseStat; 

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FABCharacterStat ModifierStat; 
};