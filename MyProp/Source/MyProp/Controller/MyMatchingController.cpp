// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/Controller/MyMatchingController.h>

#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/UI/Common/MyMatchingHUD.h>

//void AMyMatchingController::BeginPlay() {
//
//	//HUD �����ϱ�
//	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	if (GI != nullptr) {
//		m_MatchingMainHUDClass = GI->GetMatchingHUDClass();
//		m_MatchingMainHUD = Cast<UMyMatchingHUD>(CreateWidget(this, m_MatchingMainHUDClass));
//		if (nullptr != m_MatchingMainHUD) m_MatchingMainHUD->AddToViewport();
//	}
//
//	//// InputMode ����
//	//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
//	//FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
//	//Controller->SetInputMode(mode); //�����ʿ�
//
//	PosessToPawn();
//
//}
//
//void AMyMatchingController::PosessToPawn() //���� ī�޶� pawn ���� �Ҵ�ǵ��� �ϱ�
//{
//
//
//	for (TObjectIterator<APawn> e; e; ++e) 
//	{
//		if (e->Tags[0].ToString().Equals("MatchingCamera")) 
//		{
//			//�÷��̾���� ���� ���� ���� ī�޶� ���Ϳ��� Posess �����ϱ�
//			GetWorld()->GetFirstPlayerController()->Possess(*e);
//			break;
//		}
//	}
//}
//
//void AMyMatchingController::UpdatePlayerNum() //UI �󿡼� �÷��̾� ���� �ݿ��ϱ�
//{
//
//}
//
//void AMyMatchingController::PlayerTick(float DeltaTime) {
//
//	UpdatePlayerNum(); //�÷��̾� ���� �ݿ��ϱ�
//
//}
