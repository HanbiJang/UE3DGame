// Fill out your copyright notice in the Description page of Project Settings.

#include "MyTimerWidget.h"

#include <MyProp/Mode/MyPropGameModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include "Kismet/GameplayStatics.h"


void UMyTimerWidget::NativeConstruct() {
	Super::NativeConstruct();

	//�ð� ǥ�� �ؽ�Ʈ UI ��������
	m_TimeText = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimeText")));

	m_MachineImgArr.SetNum(5);
	for (int i = 0; i < m_MachineImgArr.Num(); i++) {
		FString str = FString::Printf(TEXT("MachineImg_%i"), i+1);
		m_MachineImgArr[i] = Cast<UImage>(GetWidgetFromName(*str));
	}

	//�̹��� ��������
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		UTexture2D* DoneImgAsset = Cast<UTexture2D>(GI->GetDoneImg());
		if (DoneImgAsset) DoneImg = DoneImgAsset;
	}



}
void UMyTimerWidget::NativeTick(const FGeometry& Geometry, float DT) {
	Super::NativeTick(Geometry, DT);
}


void UMyTimerWidget::SetTimeText(const FString& _Name)
{
	if (nullptr != m_TimeText)
	{
		m_TimeText->SetText(FText::FromString(_Name)); //_Name���� FText�� �����Ѵ�
	}
}


void UMyTimerWidget::SetMachineImge_Done(int idx) {
	if(DoneImg)
		m_MachineImgArr[idx]->SetBrushFromTexture(DoneImg);

	else {
		//GI���� �̹��� ��������
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI != nullptr) {
			UTexture2D* DoneImgAsset = Cast<UTexture2D>(GI->GetDoneImg());
			if (DoneImgAsset) {
				DoneImg = DoneImgAsset;
				m_MachineImgArr[idx]->SetBrushFromTexture(DoneImg);
			}
		}

	}
}
