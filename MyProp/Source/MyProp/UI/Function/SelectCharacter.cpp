// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacter.h"
#include <MyProp/UI/MyStartGameWidget.h>
#include "Kismet/GameplayStatics.h"

void UMyStartGameWidget::RandomBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	//GetRandomBtn()->BackgroundColor = FLinearColor(1.0f,0.1f,0.1f); //red �ȵ�


	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->m_SelectType = EPLAYER_TYPE::RANDOM;
	}
}
void UMyStartGameWidget::KillerBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->m_SelectType = EPLAYER_TYPE::KILLER;
	}
}
void UMyStartGameWidget::SurvivorBtnPressed() {
	//�� ��ȭ
	GetRandomBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetKillerBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.75f, 0.75f));
	GetSurvivorBtn()->SetBackgroundColor(FLinearColor(1.0f, 0.1f, 0.1f));

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		GI->m_SelectType = EPLAYER_TYPE::SURVIVOR;
	}
}
void UMyStartGameWidget::StartGameBtnPressed() {
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("InGame"));
}