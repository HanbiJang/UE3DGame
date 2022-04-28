// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStartModeBase.h"

AMyStartModeBase::AMyStartModeBase() {
	ConstructorHelpers::FClassFinder<UUserWidget> HUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/StartUI/BP_MyStartHUD.BP_MyStartHUD_C'"));
	if (HUD.Succeeded())
	{
		m_HUDClass = HUD.Class;
	}

	DefaultPawnClass = nullptr;
}

void AMyStartModeBase::BeginPlay() {
	Super::BeginPlay();

	// HUD ȭ�鿡 ���
	m_HUD = Cast<UMyStartHUD>(CreateWidget(GetWorld(), m_HUDClass));

	if (nullptr == m_HUD)
		return;

	m_HUD->AddToViewport();
	m_HUD->GetStartGameWidget()->SetVisibility(ESlateVisibility::Hidden); // ���� ���� - ȭ�鿡�� �Ⱥ��̰� �ϱ�
	
	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();		
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.
	
}