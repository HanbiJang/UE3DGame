// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"

AMyPropGameModeBase::AMyPropGameModeBase() {

	//ĳ���� �������Ʈ Ŭ���� ��������
	//������Ÿ�� ���� �������Ʈ
		//Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor'

	ConstructorHelpers::FClassFinder<APawn>
		MainCharacterClass(TEXT("Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor_C'"));
	if (MainCharacterClass.Succeeded())
	{
		DefaultPawnClass = MainCharacterClass.Class;
	}

	//����UI ��������
	//_C �������ֱ�!
	ConstructorHelpers::FClassFinder<UUserWidget> MainHUD(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'"));
	if (MainHUD.Succeeded())
	{
		m_MainHUDClass = MainHUD.Class;
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//���� UI �����ϱ�
	m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));

	if (nullptr != m_MainHUD)
		m_MainHUD->AddToViewport();
}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	//ü�� UI
	UMyHPBarWidget* pHPHUD = m_MainHUD->GetHPHUD();

	//���׹̳� UI
	UMySPWidget* pSPHUD = m_MainHUD->GetSPHUD();

	if (pHPHUD) {
		pHPHUD->SetHP(_CurHPRatio);
		pHPHUD->SetText(TEXT("MyPlayerName"));
	}
	if (pSPHUD) {
		pSPHUD->SetSP(_CurSPRatio);
	}
}