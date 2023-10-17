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
	virtual void PostInitializeComponents() override; // ������ ������ �������Ǵ� ������ ȣ��Ǵ� �Լ�

protected:
	UPROPERTY(VisibleAnywhere,Category = Box)
	TObjectPtr<class UBoxComponent> Trigger; // ��Ʈ ������Ʈ�� �ش��ϴ� �ڽ� ������Ʈ

	UPROPERTY(VisibleAnywhere,Category = Box)
	TObjectPtr<class UStaticMeshComponent> Mesh; // ����ƽ �޽��� ������ ����ƽ �޽� ������Ʈ

	UPROPERTY(VisibleAnywhere,Category = Effect)
	TObjectPtr<class UParticleSystemComponent> Effect; // ����Ʈ�� ����� ��ƼŬ �ý��� ������Ʈ

	UPROPERTY(EditAnywhere, Category = Item)
	TObjectPtr<class UABItemData> Item; // �� ������ �������� �θ� Ŭ���� ���� UABItemData �� ������ ������ �����Ͽ� ������ ���� �� �� �ϳ��� ���⿡ ������ �� ����

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* ParticleSystem);
};
