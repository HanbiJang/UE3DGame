// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/UI/Common/MyMatchingHUD.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/Mode/MyMatchingModeBase.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Controller/MyPlayerController.h>

void UMyMatchingHUD::NativeConstruct() {
	
	//UI��������
	TB_MatchingPlayer = Cast<UTextBlock>(GetWidgetFromName(TEXT("TB_MatchingPlayer")));
	
	//���� �ν��Ͻ� ��������
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


}
void UMyMatchingHUD::NativeTick(const FGeometry& Geometry, float DT) {

	AMyPlayerController* pc = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc) 
	{
		UpdatePlayerNum_Server(pc->iPlayerCnt);
	}
	

}

void UMyMatchingHUD::UpdatePlayerNum_Server_Implementation(int num) {
	//�ο����� �����ν��Ͻ��� �÷��̾� ���� ������Ʈ�ϱ�
	
	//�ؽ�Ʈ ������Ʈ
	if (GI) {
		FString str = FString::Printf(TEXT("%d / %d"), num, GI->maxPlayer);
		//FString str = FString::Printf(TEXT("%d�� ... %d��"), (int)CurPlayerNum, (int)GI->maxPlayer); //�ѱ��� �Ұ�
		TB_MatchingPlayer->SetText(FText::FromString(str));
	}
}