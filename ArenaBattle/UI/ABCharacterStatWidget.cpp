// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// ������ �ʱ�ȭ �� ��, FABCharacterStat�̶�� ����ü�� ������ �Ӽ����� ������ ��� �о, ���⿡ ��Ī�Ǵ� �ؽ�Ʈ ����� �����͸� ���������� �Ѵ�
	
	// ����ü�� ���� �Ӽ����� ���� ��� �̾��ִ� ���ͷ����͸� Ȱ���Ѵ�
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName()); // �Ӽ��� ���� Ű���� ������ ����
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName()); // ���̽� ���Ȱ��� �ش��ϴ� �ؽ�Ʈ ��� ��Ʈ���� ��� ���� �ؽ�Ʈ ��Ʈ�� �̸� ����
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName()); // ������̾� ���Ȱ��� �ش��ϴ� �ؽ�Ʈ ��� ��Ʈ���� ��� ���� �ؽ�Ʈ ��Ʈ�� �̸� ����
		
		// �ϳ��� Ű�� ���� ���̽� ���� �ؽ�Ʈ ���, ������̾� ���� �ؽ�Ʈ ��Ͽ� ���� ���� �� �ֵ��� �� ���� Lookup���� ����Ѵ�
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UABCharacterStatWidget::UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat)
{
	// ������Ʈ �� Lookup �ڷᱸ���� ���캸�� �ش� ��Ʈ���� ���� ������Ʈ �ϸ� �ȴ�.
	
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt) //FABCharacterStat ����ü�� �Ӽ����� ���������� ��ȸ
	{
		const FName PropKey(PropIt->GetName()); // �ش� �Ӽ� �̸��� ���� Ű���� ����

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData); //GetValue_InContainer �Լ��� ����� BaseStat���� ��ȸ�ϴ� ���� �Ӽ��� ������ �Ӽ��� ���� ������ BaseData�� ���� 
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData); //GetValue_InContainer �Լ��� ����� ModifierStat���� ��ȸ�ϴ� ���� �Ӽ��� ������ �Ӽ��� ���� ������ ModifierData�� ����

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey); // �ش� �Ӽ��� Ű���� ����� ������ �����ص� BaseLookup���� ���� ������ �ؽ�Ʈ����� ��´�
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData))); 
			// �� �ش� �Ӽ��� �����͸� �ؽ�Ʈ����� �ؽ�Ʈ�� ����. SanitizeFloat �Լ��� ���� �ε� �Ҽ��� ���ڸ� 
			// ���ڿ��� ��ȯ�ϰ� FromString �Լ��� ���ؼ� ���ڿ��� FText�� ��ȯ�Ѵ�.
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}	
	}
}
