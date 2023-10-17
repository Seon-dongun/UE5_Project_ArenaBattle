#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABItemBox.generated.h"

UCLASS()
class ARENABATTLE_API AABItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AABItemBox();

	FORCEINLINE class UBoxComponent* GetTrigger() { return Trigger; }

protected:
	virtual void PostInitializeComponents() override; // 액터의 세팅이 마무리되는 시점에 호출되는 함수

protected:
	UPROPERTY(VisibleAnywhere,Category = Box)
	TObjectPtr<class UBoxComponent> Trigger; // 루트 컴포넌트에 해당하는 박스 컴포넌트

	UPROPERTY(VisibleAnywhere,Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh; // 스태틱 메쉬를 보여줄 스태틱 메쉬 컴포넌트

	UPROPERTY(VisibleAnywhere,Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect; // 이펙트를 재생할 파티클 시스템 컴포넌트

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item; // 세 종류의 아이템의 부모 클래스 형인 UABItemData 의 포인터 변수로 지정하여 아이템 종류 셋 중 하나가 여기에 대응될 수 있음

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
