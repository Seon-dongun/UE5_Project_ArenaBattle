// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABCharacterStatWidget.h"
#include "Components/TextBlock.h"

void UABCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 위젯이 초기화 될 때, FABCharacterStat이라는 구조체가 가지는 속성들의 값들을 모두 읽어서, 여기에 매칭되는 텍스트 블록의 포인터를 가져오도록 한다
	
	// 구조체가 가진 속성들의 값을 모두 뽑아주는 이터레이터를 활용한다
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName()); // 속성에 대한 키값을 가지는 변수
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName()); // 베이스 스탯값에 해당하는 텍스트 블록 컨트롤을 얻기 위한 텍스트 컨트롤 이름 변수
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName()); // 모디파이어 스탯값에 해당하는 텍스트 블록 컨트롤을 얻기 위한 텍스트 컨트롤 이름 변수
		
		// 하나의 키를 통해 베이스 스탯 텍스트 블록, 모디파이어 스탯 텍스트 블록에 대해 얻을 수 있도록 두 개의 Lookup맵을 사용한다
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
	// 업데이트 시 Lookup 자료구조를 살펴보고 해당 컨트롤의 값을 업데이트 하면 된다.
	
	for (TFieldIterator<FNumericProperty> PropIt(FABCharacterStat::StaticStruct()); PropIt; ++PropIt) //FABCharacterStat 구조체의 속성들을 순차적으로 순회
	{
		const FName PropKey(PropIt->GetName()); // 해당 속성 이름에 따른 키값을 얻음

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData); //GetValue_InContainer 함수를 사용해 BaseStat에서 순회하는 현재 속성과 동일한 속성의 값만 가져와 BaseData에 저장 
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData); //GetValue_InContainer 함수를 사용해 ModifierStat에서 순회하는 현재 속성과 동일한 속성의 값만 가져와 ModifierData에 저장

		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey); // 해당 속성의 키값을 사용해 이전에 저장해둔 BaseLookup맵을 통해 적절한 텍스트블록을 얻는다
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData))); 
			// 얻어낸 해당 속성의 데이터를 텍스트블록의 텍스트로 지정. SanitizeFloat 함수를 통해 부동 소수점 숫자를 
			// 문자열로 변환하고 FromString 함수를 통해서 문자열을 FText로 변환한다.
		}

		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}	
	}
}
