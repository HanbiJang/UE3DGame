// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	//ĳ���� �������Ʈ Ŭ���� ��������
	ConstructorHelpers::FClassFinder<APawn>
		Survivor(TEXT("Blueprint'/Game/Blueprints/Survivor/BP_Survivor.BP_Survivor_C'"));
	ConstructorHelpers::FClassFinder<APawn>
		Killer(TEXT("Blueprint'/Game/Blueprints/Killer/BP_Killer.BP_Killer_C'"));

	//���� ���� ��������
	EPLAYER_TYPE m_SelectType;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SelectType = GI->GetSelectType();
		UE_LOG(LogTemp, Log, TEXT("m_SelectTypem_SelectType"));
	}

	//[1] ������ �ڵ�
	switch (m_SelectType)
	{
	case EPLAYER_TYPE::KILLER:
		if (Killer.Succeeded()) DefaultPawnClass = Killer.Class;
		break;
	case EPLAYER_TYPE::SURVIVOR:
		if (Survivor.Succeeded()) DefaultPawnClass = Survivor.Class;
		break;
	case EPLAYER_TYPE::RANDOM:
		int irandom = rand() % 2;
		if (irandom == 0) {
			//���θ�
			if (Killer.Succeeded()) DefaultPawnClass = Killer.Class;
		}
		else {
			//������
			if (Survivor.Succeeded()) DefaultPawnClass = Survivor.Class;
		}
		break;
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
		pHPHUD->SetText(TEXT("Survivor 1"));
	}
	if (pSPHUD) {
		pSPHUD->SetSP(_CurSPRatio);
	}
}