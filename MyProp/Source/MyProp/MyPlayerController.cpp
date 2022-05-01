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
		m_SurvivorMainHUD->AddToViewport();
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;
	Controller->SetInputMode(mode); //�����ʿ�
	Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
}

void AMyPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	//���¹̳� UI ������Ʈ


}

void AMyPlayerController::ClientM_Implementation() {
	FString str = FString::Printf(TEXT("Controller UIUI yesyes"));
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, str); // ȭ�����
}



