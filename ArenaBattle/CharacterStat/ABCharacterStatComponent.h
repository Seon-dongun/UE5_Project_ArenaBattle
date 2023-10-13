#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/ABCharacterStat.h"
#include "ABCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate); // HP가 0일 때 죽었다라는 시그널을 보내는 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/); // 변경된 현재 HP 값을 구독한 객체들에게 보내는 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FABCharacterStat& /*BaseStat*/, const FABCharacterStat& /*ModifierStat*/);// 스탯이 수정될 때마다 알림을 보내는 델리게이트

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
	// 최대한 부하를 줄이기 위해 Tick 함수는 호출하지 않고 필요할 때만 호출되도록 할 것이다

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel); // 레벨 정보 Set
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; } // 레벨 정보 Get
	FORCEINLINE void AddBaseStat(const FABCharacterStat& InAddBaseStat) { BaseStat = BaseStat+InAddBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());} // 추가 스탯을 더해주고 스탯 변화에 따른 델리게이트 알림 보내기
	FORCEINLINE void SetBaseStat(const FABCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(),GetModifierStat()); } //Base Stat Set
	FORCEINLINE void SetModifierStat(const FABCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(),GetModifierStat()); } //Modifier Stat Set
	FORCEINLINE FABCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; } // 스탯 총 합 Get
	FORCEINLINE float GetAttackRadius() const { return AttackRadius; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; };
	FORCEINLINE void HealHp(float InHealAmount) { CurrentHp = FMath::Clamp(CurrentHp + InHealAmount, 0, GetTotalStat().MaxHp); OnHpChanged.Broadcast(CurrentHp); } // 최대 체력값까지만 회복될 수 있도록 제한 설정하고 체력이 변화했으니 체력 변화에 대한 델리게이트 알림 보내기
	float ApplyDamage(float InDamage); // 데미지를 받았을 때 데미지를 적용하는 함수

	// 베이스 스탯, 모디파이어 스탯 Getter 함수 추가
	FORCEINLINE const FABCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE const FABCharacterStat& GetModifierStat() const { return ModifierStat; }


protected:
	// HP 값이 변동되었을 떄 실행될 함수. HP 변동 시 반드시 이 함수를 호출하도록 설정
	void SetHp(float NewHp);

	/* 
		기존 코드에 있던 MaxHp 변수는 기본 스텟에 있는 변수이므로 삭제
	*/

	// 아래 4개의 값은 게임할 때마다 새롭게 지정되기 때문에 디스크에 저장할 필요가 없을 수 있다. 이와 같이 디스크에 저장할 필요가 없다면
	// Transient 키워드를 추가하여 디스크로부터 저장할 때 불필요한 공간이 낭비하지 않도록 지정해줄 수 있음
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient,VisibleInstanceOnly,Category = Stat)
	float CurrentLevel; // 현재 레벨을 기반으로 게임 싱글톤으로부터 BaseStat 정보를 제공받기 위해 필요한 현재 레벨에 대한 변수

	UPROPERTY(VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	float AttackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FABCharacterStat BaseStat; 

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"));
	FABCharacterStat ModifierStat; 
};