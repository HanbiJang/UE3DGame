// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMachineWidget.h"

void UMyMachineWidget::NativeConstruct() {
	Super::NativeConstruct();

	//ü��,�̸� UI ��������
	m_PgMachine = Cast<UProgressBar>(GetWidgetFromName(TEXT("PgMachine")));
	m_MachineText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MachineText")));

}
void UMyMachineWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}

void UMyMachineWidget::SetPgMachine(float _fRatio)
{
	if (nullptr != m_PgMachine)
	{
		m_PgMachine->SetPercent(_fRatio);
	}
}

void UMyMachineWidget::SetMachineText(const FString& _Name)
{
	if (nullptr != m_MachineText)
	{
		m_MachineText->SetText(FText::FromString(_Name)); //_Name���� FText�� �����Ѵ�
	}
}
