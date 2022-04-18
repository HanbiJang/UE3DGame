// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMainHUD.h"

//���� �� ����
void UMyMainHUD::NativeConstruct() {
	Super::NativeConstruct();

	//UI ��������
	//���׹̳�
	m_SPWidget = Cast<UMySPWidget>(GetWidgetFromName(TEXT("BP_MySP")));
	//ü�� 
	m_HPWidget = Cast<UMyHPBarWidget>(GetWidgetFromName(TEXT("BP_MyHP")));

}


//ƽ���� �����
void UMyMainHUD::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);

}
