// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/Controller/MyMatchingController.h>

#include <MyProp/GameInstance/MyGameInstance.h>

void AMyMatchingController::BeginPlay() {

	//HUD �����ϱ�
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_MatchingMainHUDClass = GI->GetMatchingHUDClass();
		m_MatchingMainHUD = Cast<UMyMatchingHUD>(CreateWidget(this, m_MatchingMainHUDClass));
		if (nullptr != m_MatchingMainHUD) m_MatchingMainHUD->AddToViewport();
	}

	//// InputMode ����
	//APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	//FInputModeGameOnly mode; //UI Ŭ�� �Ұ�
	//Controller->SetInputMode(mode); //�����ʿ�


}

void AMyMatchingController::PlayerTick(float DeltaTime) {

}
