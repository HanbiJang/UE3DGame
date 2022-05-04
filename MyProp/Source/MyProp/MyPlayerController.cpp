// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/MyGameInstance.h>

#include <MyProp/Mode/MyPropGameModeBase.h>

void AMyPlayerController::DrawHUD_Client_Implementation() {
	//UI
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SurvivorMainHUDClass = GI->GetSurvivorWidgetClass();

		m_SurvivorMainHUD = Cast<UMyMainHUD>(CreateWidget(this, m_SurvivorMainHUDClass));
		if (nullptr != m_SurvivorMainHUD) m_SurvivorMainHUD->AddToViewport();
	}

	//// InputMode ����
	//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI mode;
	//Controller->SetInputMode(mode); //�����ʿ�
	//Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI mode;
	FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
	Controller->SetInputMode(mode); //�����ʿ�
	//Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	//���¹̳� UI ������Ʈ


}
void AMyPlayerController::DrawHUD_Server_Implementation() {
	//UI ����
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_KillerMainHUDClass = GI->GetKillerWidgetClass();

		m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_KillerMainHUDClass));
		if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameAndUI mode;
	FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
	Controller->SetInputMode(mode);
	//Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
}


void AMyPlayerController::UpdatePlayHUD_Killer_Implementation(float _CurHPRatio, float _CurSPRatio) {

}

void AMyPlayerController::UpdatePlayHUD_Survivor_Implementation(float _CurHPRatio, float _CurSPRatio, float _CurHP, float _MaxHP) {

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



