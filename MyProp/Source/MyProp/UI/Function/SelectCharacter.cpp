// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include <MyProp/UI/MyStartGameWidget.h>
#include "Kismet/GameplayStatics.h"
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/Mode/MyStartModeBase.h>
#include <MyProp/Controller/MyMatchingController.h>


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
		//�ε� ȭ�� �����ֱ�
		m_LoadingHUD = (CreateWidget(GetWorld(), GI->m_LoadingWidget));
		if (nullptr != m_LoadingHUD) {
			m_LoadingHUD->AddToViewport();
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

	//�񵿱� �ε� ���� ���� ���� �̸� �ؾ���
	//���Ӹ�
	//LoadPackageAsync(TEXT("/Game/Levels/InGameMap"),
	//	FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
	//		{
	//			if (result == EAsyncLoadingResult::Succeeded) {
	//				OnLoadingComplete_Server(SessionSettings);
	//			}
	//		}
	//	),
	//	0,
	//			PKG_ContainsMap);

	//��Ī��
	LoadPackageAsync(TEXT("/Game/Levels/Prototype/MatchingMap"),
		FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
			{
				if (result == EAsyncLoadingResult::Succeeded) {
					OnLoadingComplete_Server(SessionSettings);
				}
			}
		),
		0,
				PKG_ContainsMap);

	//SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
}
//���� ã�� (������) 
void UMyStartGameWidget::FindServer() {

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true; //Is Lan
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Find Session Start"));
}

void UMyStartGameWidget::Init() {
	if (IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get()) {
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			//���ε� - Server - ���� ���� Create�� �Ϸ�Ǿ��� ��
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnCreateSessionComplete);
			//���ε�  - Client - ���� ���� Find�� �Ϸ�Ǿ��� ��
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnFindSessionComplete);
			//���ε� - Client - ���ӿ� Join�� �Ϸ�Ǿ��� �� 
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMyStartGameWidget::OnJoinSessionComplete);
		}
	}
}

void UMyStartGameWidget::OnCreateSessionComplete(FName ServerName, bool Succeded) {
	
	if (Succeded) {


		//UE_LOG(LogTemp, Log, TEXT("Create %s Session Success: %d"), *ServerName.ToString(), Succeded);
		//"World'/Game/Levels/InGameMap.InGameMap'" (���� �ּ�) listen =������ ����
		//GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
		
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Blue, TEXT("Create Session Success")); // Server ���� ����
		GetWorld()->ServerTravel("/Game/Levels/Prototype/MatchingMap?listen"); //��Ī ȭ������ �ǳʰ���
	}
}

#pragma region asynchronous-loading

void UMyStartGameWidget::OnLoadingComplete_Server(FOnlineSessionSettings SessionSettings) {
	//(����) �ε� �Ϸ�Ǿ��� ���� �Լ�
	SessionInterface->CreateSession(0, FName("My Session"), SessionSettings);
	
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGameMap"),true,(FString)(L"listen"));

	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("OnLoadingComplete_Server"));
	//GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");

}

//void UMyStartGameWidget::OnLoadingComplete_Client(APlayerController* pc, FString joinAddress, ETravelType type) {
//	
//	//�ε� �Ϸ�Ǿ��� ���� �Լ�
//	//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Game Start!"));
//	Cast<AMyPlayerController>(pc)->ClientTravel(joinAddress, type);
//
//}

//void UMyStartGameWidget::OnLoadingComplete(const FName&, UPackage*, EAsyncLoadingResult::Type result) {
//	//�ε� �Ϸ�Ǿ��� ���� �Լ�
//	if (result == EAsyncLoadingResult::Succeeded) {
//		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("succee"));
//		GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
//		//GetWorld()->GetTimerManager().SetTimer(LoadingTimerHandler, this, &UMyStartGameWidget::DelayServerTravel, 3.0f, false);
//	}
//	else {
//		//GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, TEXT("Fail"));
//	}
//	
//}


//void UMyStartGameWidget::DelayServerTravel() {
//
//	//�ε� �Ϸ�Ǿ��� ���� �Լ�
//	GetWorld()->ServerTravel("/Game/Levels/InGameMap?listen");
//
//}

//void UMyStartGameWidget::DelayClientTravel() {
//	
//	//���� ����
//}

#pragma endregion

void UMyStartGameWidget::OnFindSessionComplete(bool Succeded) {
	
	if (Succeded) {

		TArray<FOnlineSessionSearchResult> SearchResults;

		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Find Session Success"));
		FString str = FString::Printf(TEXT("Serch result : server count %d"), SearchResults.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, str);

		SearchResults = SessionSearch->SearchResults;
		if (SearchResults.Num() >= 1)  //Find ����� �ִٸ�
		{ 	
			SessionInterface->JoinSession(0, "My Session", SearchResults[0]); //���� : ���� ������ �ִٸ� ��Ƽ�÷��̸� �����ۿ� ����
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("JoinSession() Start"));
		}
		else { //Find ��� ���ٸ� �ٽ� Find �ϱ�
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("No Server to Join"));
			FindServer();
			//SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		}

;
	}	

}

void UMyStartGameWidget::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Join Session Complete"));

	//StartLevel ���� ���ÿ��� AMyPlayerController�� ��Ʈ�ѷ� ����
	APlayerController* playerCont = UGameplayStatics::GetPlayerController(GetWorld(), 0); //�÷��̾� ��Ʈ�ѷ� �������� => StartMap�� ��Ʈ�ѷ��� AMyController Ÿ��	

	if (playerCont) {
		
		AMyPlayerController* m_PC = Cast<AMyPlayerController>(playerCont);

		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName , JoinAddress);

		if (JoinAddress != "") { //Join�� ������ �ּҸ� ã���� ���

			if (m_PC) {

				////�÷��̾� �̸� ����
				//UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
				//if (GI) {
				//	Cast<AMyPlayerController>(PC)->SetMyPlayerName(GI->MyPlayerName);
				//}
				
				////�񵿱� �ε�
				//LoadPackageAsync(TEXT("/Game/Levels/InGameMap"),
				//	FLoadPackageAsyncDelegate::CreateLambda([=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type result)
				//		{
				//			if (result == EAsyncLoadingResult::Succeeded) {
				//				Cast<AMyPlayerController>(PC)->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
				//				//OnLoadingComplete_Client(PC, JoinAddress, ETravelType::TRAVEL_Absolute);
				//			}
				//			else {
				//				GEngine->AddOnScreenDebugMessage(0, 5, FColor::Blue, TEXT("Join Failed"));

				//				//�ٽ� ã��
				//				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
				//			}
				//		}
				//	),
				//	0,
				//	PKG_ContainsMap);

				
				m_PC->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Black, TEXT("Join Complete - Client Travel Start"));

			}
			else 
			{ 
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("playerCont Can't Cast to AMyPlayerController")); 
			}			
			
		}
		else //Join Address�� ���� ������
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("JoinAddress null"));
		}
			
	}

}