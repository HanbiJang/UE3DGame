// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include <MyProp/UI/MyStartGameWidget.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/GameInstance/MyGameInstance.h>


void UMyStartGameWidget::RandomBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	//GetRandomBtn()->BackgroundColor = FLinearColor(1.0f,0.1f,0.1f); //red �ȵ�


	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::RANDOM);
	}
}
void UMyStartGameWidget::KillerBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::KILLER);
	}
}
void UMyStartGameWidget::SurvivorBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->SetSelectType(EPLAYER_TYPE::SURVIVOR);
	}
}
void UMyStartGameWidget::StartGameBtnPressed() {

	//����ȭ
	GetStartGameBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));

	//����Ʈ ���� ��������
	EPLAYER_TYPE m_SelectType;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SelectType = GI->GetSelectType();

		if (m_SelectType == EPLAYER_TYPE::RANDOM) {
			int irandom = rand() % 2;
			if (irandom == 0) {
				//���θ��� ����
				GI->SetSelectType(EPLAYER_TYPE::KILLER);
			}
			else {
				//�����ڷ� ���� ����
				GI->SetSelectType(EPLAYER_TYPE::SURVIVOR);
			}
			m_SelectType = GI->GetSelectType();
		}	
	}
	
	//������ ĳ���� Ÿ�Կ� ���� ������ �����ϰų� ������ �����ϱ�(ã��)
	if (m_SelectType == EPLAYER_TYPE::KILLER) {
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGameMap"), true, TEXT("Listen")); //����
		
		CreateServer();
	}
		

	else if (m_SelectType == EPLAYER_TYPE::SURVIVOR) {
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("127.0.0.1")); //���� Ŭ���̾�Ʈ
		//�ε��� UI

		FindServer();
	}
}
//���� ����� (ų��)
void UMyStartGameWidget::CreateServer() {

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if(GI) SessionSettings.NumPublicConnections = GI->maxPlayer;
	else SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("MySession"), SessionSettings);
}
//���� ã�� (������) 
void UMyStartGameWidget::FindServer() {


}

void UMyStartGameWidget::Init() {
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			//���ε� - ���� ���� ������ �Ϸ�Ǿ��� ��
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnCreateSessionComplete);
		}
	}
}

void UMyStartGameWidget::OnCreateSessionComplete(FName ServerName, bool Succeded) {
	UE_LOG(LogTemp, Log, TEXT("Create %s Session Success: %d"), *ServerName.ToString() ,Succeded);
	if (Succeded) {
		//"World'/Game/Levels/InGameMap.InGameMap'" (���� �ּ�) listen =������ ����
		GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
	}
}