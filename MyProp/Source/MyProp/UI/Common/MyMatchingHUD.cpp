// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/UI/Common/MyMatchingHUD.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/Mode/MyMatchingModeBase.h>

void UMyMatchingHUD::NativeConstruct() {
	
	//UI��������
	TB_MatchingPlayer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_MatchingPlayer")));
	
	//���� �ν��Ͻ� ��������
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI) {
		CurPlayerNum = GI->curPlayerNum;
	}

}
void UMyMatchingHUD::NativeTick(const FGeometry& Geometry, float DT) {

	UpdatePlayerNum();

}

void UMyMatchingHUD::UpdatePlayerNum_Implementation() {
	//�ο����� �����ν��Ͻ��� �÷��̾� ���� ������Ʈ�ϱ�
	AMyMatchingModeBase* m_mode = Cast<AMyMatchingModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	//CurPlayerNum = GI->curPlayerNum;
	CurPlayerNum = m_mode->GetNumPlayers();
	
	//�ؽ�Ʈ ������Ʈ
	FString str = FString::Printf(TEXT("%d / %d"), CurPlayerNum, GI->maxPlayer);
	//FString str = FString::Printf(TEXT("%d�� ... %d��"), (int)CurPlayerNum, (int)GI->maxPlayer); //�ѱ��� �Ұ�
	TB_MatchingPlayer->SetText(FText::FromString(str));
}