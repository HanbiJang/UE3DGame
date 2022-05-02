// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHPBarWidget.h"

void UMyHPBarWidget::NativeConstruct() {
	Super::NativeConstruct();

	//ü��,�̸� UI ��������
	m_HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	m_PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
	m_CurHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("CurHP")));
	m_MaxHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("MaxHP")));
}
void UMyHPBarWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}

void UMyHPBarWidget::SetHP(float _fRatio)
{
	if (nullptr != m_HPBar)
	{
		m_HPBar->SetPercent(_fRatio);
	}
}

void UMyHPBarWidget::SetText_PlayerName(const FString& _Name)
{
	if (nullptr != m_PlayerName)
	{
		m_PlayerName->SetText(FText::FromString(_Name)); //_Name���� FText�� �����Ѵ�
	}
}


void UMyHPBarWidget::SetText_CurHP(const FString& _Value)
{
	if (nullptr != m_CurHP)
	{
		m_CurHP->SetText(FText::FromString(_Value)); //
	}
}
void UMyHPBarWidget::SetText_MaxHP(const FString& _Value)
{
	if (nullptr != m_MaxHP)
	{
		m_MaxHP->SetText(FText::FromString(_Value)); //
	}
}
