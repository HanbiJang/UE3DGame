// Fill out your copyright notice in the Description page of Project Settings.

#include <MyProp/UI/Common/MyOtherPlayerWidget.h>
#include "MyOtherPlayerWidget.h"

#include <MyProp/Mode/MyPropGameModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include "Kismet/GameplayStatics.h"

void UMyOtherPlayerWidget::SetText_OtherPlayerNames(TArray<FString> nameArr)
{
	for(int i =0; i< m_OtherPlayerNameArr.Num(); i++){
		if (nullptr != m_OtherPlayerNameArr[i])
		{
			m_OtherPlayerNameArr[i]->SetText(FText::FromString(nameArr[i])); //_Name���� FText�� �����Ѵ�
		}
	}

}

void UMyOtherPlayerWidget::SetOtherPlayerImg(int PlayerIdx, int ImgIdx)
{
	if (PlayerImg[ImgIdx])
		m_PlayerImgArr[PlayerIdx]->SetBrushFromTexture(PlayerImg[ImgIdx]);

	else {
		//GI���� �̹��� ��������
		UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (GI != nullptr) {
			UTexture2D* PlayerImgAsset = Cast<UTexture2D>(GI->GetPlayerImg(ImgIdx));
			if (PlayerImgAsset) {
				PlayerImg[ImgIdx] = PlayerImgAsset;
				m_PlayerImgArr[PlayerIdx]->SetBrushFromTexture(PlayerImg[ImgIdx]);
			}
		}

	}
}

void UMyOtherPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//�÷��̾� �̸� ǥ�� �ؽ�Ʈ UI ��������
	m_OtherPlayerNameArr.SetNum(4);
	for (int i = 0; i < m_OtherPlayerNameArr.Num(); i++) {
		FString str = FString::Printf(TEXT("OtherPlayerName_%i"), i + 1);
		m_OtherPlayerNameArr[i] = Cast<UTextBlock>(GetWidgetFromName(*str));
	}

	//�÷��̾� ���µ� �̹��� ��������
	m_PlayerImgArr.SetNum(4);
	for (int i = 0; i < m_PlayerImgArr.Num(); i++) {
		FString str = FString::Printf(TEXT("OtherPlayerImg_%i"), i + 1);
		m_PlayerImgArr[i] = Cast<UImage>(GetWidgetFromName(*str));
	}

	//GI���� ��� �÷��̾� �̹��� ��������
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		PlayerImg = GI->GetAllPlayerImg();
	}
}

void UMyOtherPlayerWidget::NativeTick(const FGeometry& Geometry, float DT)
{
	Super::NativeTick(Geometry, DT);
}
