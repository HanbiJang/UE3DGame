// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/UI/Common/MyTimerWidget.h>

#include <MyProp/Mode/MyPropGameModeBase.h>
#include <MyProp/Machine/MyMachine.h>
#include <MyProp/UI/Common/MyMatchingHUD.h>

AMyPlayerController::AMyPlayerController()
	//DoneMachineNum(0),
	//IsAllMachineRepaired(false)
{
	IsMatchingHUDSet = false;
	iPlayerCnt = 0;
}

void AMyPlayerController::BeginPlay() {
	Super::BeginPlay();

	//������ ���۵Ǹ� Ÿ�̸Ӹ� �θ�...�ȵ�
	//GetWorld()->GetTimerManager().SetTimer(FGameTimer, this, &AMyPlayerController::CheckTime, 1.0f, true, GameLeftTimeSec);

}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	FString str = FString::Printf(TEXT("MyPlayerName :  %s"), *MyPlayerName);
	//GEngine->AddOnScreenDebugMessage(0, 0.5, FColor::Blue, str);

	//���� ���� 00�� ���
	FString levelName = GetWorld()->GetMapName();
	if (levelName.Equals("MatchingMap")) {
		UpdatePlayerNum(iPlayerCnt); //�÷��̾� ���� �ݿ��ϱ�

		if (!IsMatchingHUDSet)
		{
			//HUD �����ϱ�

				UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
				if (GI != nullptr) {
					m_MatchingMainHUDClass = GI->GetMatchingHUDClass();
					m_MatchingMainHUD = Cast<UMyMatchingHUD>(CreateWidget(this, m_MatchingMainHUDClass));
					if (nullptr != m_MatchingMainHUD) m_MatchingMainHUD->AddToViewport();
					IsMatchingHUDSet = true;
				}

				//// InputMode ����
				//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
				//FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
				//Controller->SetInputMode(mode); //�����ʿ�

				PosessToPawn();
			
		}
	}

	else if (levelName.Equals("InGameMap")) 
	{
		//matching UI �����ϱ�
		if(m_MatchingMainHUD)
			m_MatchingMainHUD->RemoveFromViewport();
	}

}

void AMyPlayerController::SetPlayerCnt_Client_Implementation(int n) { iPlayerCnt = n; }
void AMyPlayerController::SetID_Client_Implementation(int n) { ID = n; }

void AMyPlayerController::UpdateTimerUI_Client_Implementation(const FString& timestr, int DoneMachineNum) {

	GetMainHUD()->GetTimerHUD()->SetTimeText(timestr);

	//������ ���� ���� ������ �׸� ������Ʈ
	if(DoneMachineNum >= 1)
		GetMainHUD()->GetTimerHUD()->SetMachineImge_Done(DoneMachineNum-1);

}

void AMyPlayerController::UpdateTimerUI_Server_Implementation(const FString& timestr, int DoneMachineNum) {

	GetKillerMainHUD()->GetTimerHUD()->SetTimeText(timestr);

	//������ ���� ���� ������ �׸� ������Ʈ
	if (DoneMachineNum >= 1)
		GetKillerMainHUD()->GetTimerHUD()->SetMachineImge_Done(DoneMachineNum - 1);

}

//ȭ�� �׸���========================================================
void AMyPlayerController::DrawHUD_Client_Implementation() 
{
	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
		m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(this, m_SurvivorMainHUDClass));
		if (nullptr != m_SurvivorMainHUD) m_SurvivorMainHUD->AddToViewport();

		//������ UI �����
		m_SurvivorMainHUD->GetMachineHUD()->SetVisibility(ESlateVisibility::Hidden);
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
	Controller->SetInputMode(mode); //�����ʿ�

}
void AMyPlayerController::DrawHUD_Server_Implementation() {
	//UI ����
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_KillerMainHUDClass = GI->GetKillerWidgetClass();
		m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
		if (nullptr != m_KillerMainHUD) { 
			m_KillerMainHUD->AddToViewport(); 
			
			//UI�ʱ�ȭ (ó���� ��ų ��� ����)
			//m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI mode;
	FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
	Controller->SetInputMode(mode);
	//Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
}

void AMyPlayerController::UpdatePlayHUD_Killer_Implementation(float _CurQTimeRatio, float _CurETimeRatio, float _CurRCTimeRatio
, float _CurQTime, float _CurETime, float _CurRCTime) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {

		if (m_KillerMainHUD != nullptr) {
			//���α׷����� ����
			m_KillerMainHUD->SetQSkillPg(_CurQTimeRatio);
			m_KillerMainHUD->SetESkillPg(_CurETimeRatio);
			m_KillerMainHUD->SetClickSkillPg(_CurRCTimeRatio);

			//[1]Q
			if (_CurQTime < 0.05f) {
				if(m_KillerMainHUD->GetQSkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetQSkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
		
				FString _QTime = FString::Printf(TEXT("%d"), (int)_CurQTime);
				m_KillerMainHUD->SetText_QSkill(_QTime);
				if (m_KillerMainHUD->GetQSkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetQSkillText()->SetVisibility(ESlateVisibility::Visible);
				}
			}
			//[2]E
			if (_CurETime < 0.05f) {
				if (m_KillerMainHUD->GetESkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetESkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				FString _ETime = FString::Printf(TEXT("%d"), (int)_CurETime);
				m_KillerMainHUD->SetText_ESkill(_ETime);
				if (m_KillerMainHUD->GetESkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetESkillText()->SetVisibility(ESlateVisibility::Visible);
				}

			}
			//[3]RC
			if (_CurRCTime < 0.05f) {
				if (m_KillerMainHUD->GetClickSkillText()->Visibility == ESlateVisibility::Visible)
					m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Hidden);
			}
			else {
				FString _RCTime = FString::Printf(TEXT("%d"), (int)_CurRCTime);
				m_KillerMainHUD->SetText_ClickSkill(_RCTime);
				if (m_KillerMainHUD->GetClickSkillText()->Visibility == ESlateVisibility::Hidden) {
					m_KillerMainHUD->GetClickSkillText()->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}

}

void AMyPlayerController::UpdatePlayHUD_Survivor_Implementation(
	float _CurHPRatio,
	float _CurSPRatio,
	float _CurHP, 
	float _MaxHP) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			//ü�� UI
			UMyHPBarWidget* pHPHUD = m_SurvivorMainHUD->GetHPHUD();

			//���׹̳� UI
			UMySPWidget* pSPHUD = m_SurvivorMainHUD->GetSPHUD();

			if (pHPHUD) {
				pHPHUD->SetHP(_CurHPRatio);
				pHPHUD->SetText_PlayerName(TEXT("Survivor 1"));

				//HP SP
				FString hp = FString::Printf(TEXT("%d"), (int)_CurHP);
				FString maxhp = FString::Printf(TEXT("%d"), (int)_MaxHP);

				pHPHUD->SetText_PlayerName(TEXT("Survivor1"));
				pHPHUD->SetText_CurHP(hp);
				pHPHUD->SetText_MaxHP(maxhp);
			}
			if (pSPHUD) {
				pSPHUD->SetSP(_CurSPRatio);
			}

		}
	}

}

void AMyPlayerController::UpdateMachineHUD_Survivor_Implementation(
	float _CurMachineRatio
	) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {

			//������ UI
			UMyMachineWidget* pMachineHUD = m_SurvivorMainHUD->GetMachineHUD();

			//��� ����
			if (pMachineHUD) {
				pMachineHUD->SetPgMachine(_CurMachineRatio);
			}

		}
	}

}

//��Ī ------

void AMyPlayerController::PosessToPawn() //���� ī�޶� pawn ���� �Ҵ�ǵ��� �ϱ�
{
	for (TObjectIterator<APawn> e; e; ++e)
	{
		if (e->Tags[0].ToString().Equals("MatchingCamera"))
		{
			//�÷��̾���� ���� ���� ���� ī�޶� ���Ϳ��� Posess �����ϱ�
			GetWorld()->GetFirstPlayerController()->Possess(*e);
			break;
		}
	}
}

void AMyPlayerController::UpdatePlayerNum_Implementation(int num) //UI �󿡼� �÷��̾� ���� �ݿ��ϱ�
{
	if (m_MatchingMainHUD) 
	{
		m_MatchingMainHUD->UpdatePlayerNum_Server(num);
	}
}



