// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {

	//������ ����UI ��������
	ConstructorHelpers::FClassFinder<UUserWidget> SurvivorMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'")); //_C �������ֱ�!
	//���θ� ����UI
	ConstructorHelpers::FClassFinder<UUserWidget> KillerMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/Killer/BP_KillerMainHUD.BP_KillerMainHUD_C'")); //_C �������ֱ�!
	
	DefaultPawnClass = nullptr;

	//UI
	m_SurvivorMainHUDClass = SurvivorMainHUD.Class;
	m_KillerMainHUDClass = KillerMainHUD.Class;

	////���� ���� ��������
	//EPLAYER_TYPE m_SelectType;
	//UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI != nullptr) {
	//	m_SelectType = GI->GetSelectType();
	//	//[1] ������ �ڵ�
	//	switch (m_SelectType)
	//	{
	//	case EPLAYER_TYPE::KILLER:
	//		if (HasAuthority()) {
	//			DefaultPawnClass = GI->GetKiller();
	//			if (KillerMainHUD.Succeeded()) m_KillerMainHUDClass = KillerMainHUD.Class; //UI
	//		}
	//		break;
	//	case EPLAYER_TYPE::SURVIVOR:
	//		if (!HasAuthority()) {
	//			DefaultPawnClass = GI->GetSurvivor();
	//			if (SurvivorMainHUD.Succeeded()) m_SurvivorMainHUDClass = SurvivorMainHUD.Class; //UI
	//		}
	//		break;
	//	}
	//}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//���� UI �����ϱ�
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {

		//if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
		//	m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
		//	if (nullptr != m_MainHUD) m_MainHUD->AddToViewport();
		//}
		//else {
		//	m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
		//	if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		//}	
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			//ü�� UI
			UMyHPBarWidget* pHPHUD = m_SurvivorMainHUD->GetHPHUD();

			//���׹̳� UI
			UMySPWidget* pSPHUD = m_SurvivorMainHUD->GetSPHUD();

			if (pHPHUD) {
				pHPHUD->SetHP(_CurHPRatio);
				pHPHUD->SetText(TEXT("Survivor 1"));
			}
			if (pSPHUD) {
				pSPHUD->SetSP(_CurSPRatio);
			}
		}
		else {

		}
	}
}

void AMyPropGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);

	//�÷��̾� �ο� �� ����
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->iPlayerCnt++;
		FString str = FString::Printf(TEXT("Plyer Count : %d"), GI->iPlayerCnt);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, str); // ȭ�����
	}

	//ĳ����
	FActorSpawnParameters spawnInfo; //���� ����
	spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	
	if (GI->iPlayerCnt >= 1) {

		//�÷��̾ �� ���̸�, ������ ���θ�, Ŭ��� �����ڷ� ����

		//������ ĳ���Ϳ� ���� (����)
		if (GI->iPlayerCnt == 1) {
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //���� ĳ���� ����
		}
		else if (GI->iPlayerCnt >= 2) { //Ŭ���̾�Ʈ ĳ���� ����
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
		}
		if (HasAuthority()) {
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		}
		else {
			m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_SurvivorMainHUDClass));
			if (nullptr != m_SurvivorMainHUD) m_SurvivorMainHUD->AddToViewport();
		}
	}


}