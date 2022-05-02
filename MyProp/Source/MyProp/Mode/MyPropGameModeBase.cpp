// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	DefaultPawnClass = nullptr;

	////UI
	//UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GI != nullptr) {
	//	m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();
	//	m_KillerMainHUDClass = GI->GetKillerWidgetClass();
	//}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();
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
			FString str = FString::Printf(TEXT("You Server"));

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, str); // ȭ�����
			pKillerPlayer = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(), vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);
			GetWorld()->GetFirstPlayerController()->Possess(pKillerPlayer); //���� ĳ���� ����

			//HUD ���� 
			Killer1Controller = Cast<AMyPlayerController>(NewPlayer);
			if (Killer1Controller != nullptr)
				Killer1Controller->DrawHUD_Server();
		}
		else if (GI->iPlayerCnt == 2) { //Ŭ���̾�Ʈ - ������ ĳ���� ����
			pSurvivorPlayer1 = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(), vSurvivor1SpawnLocation, FRotator(0, 0, 0), spawnInfo);
			NewPlayer->Possess(pSurvivorPlayer1);
			
			//HUD ���� 
			Survivor1Controller = Cast<AMyPlayerController>(NewPlayer);
			if (Survivor1Controller != nullptr)
				Survivor1Controller->DrawHUD_Client();
		}
	}
}