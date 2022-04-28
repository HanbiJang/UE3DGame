// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>
#include <MyProp/MyPlayerController.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	DefaultPawnClass = nullptr;

	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
		m_KillerMainHUDClass = GI->GetKillerWidgetClass();
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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
		if (GI->iPlayerCnt == 1) { //���� - ���θ� ĳ���� ����
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //���� ĳ���� ����

			//UI ����
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();

			// InputMode ����
			APlayerController* Controller = GetWorld()->GetFirstPlayerController();
			FInputModeGameAndUI mode;
			Controller->SetInputMode(mode);
			Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
		}
		else if (GI->iPlayerCnt == 2) { //Ŭ���̾�Ʈ - ������ ĳ���� ����
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
			
			//HUD ���� 
			AMyPlayerController* newPlayer_ = Cast<AMyPlayerController>(NewPlayer);
			if (newPlayer_ != nullptr)
				newPlayer_->DrawHUD_Client();

			// InputMode ����
			APlayerController* Controller = NewPlayer;
			FInputModeGameAndUI mode;
			Controller->SetInputMode(mode); //�����ʿ�
			Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
		}
		
	}
}