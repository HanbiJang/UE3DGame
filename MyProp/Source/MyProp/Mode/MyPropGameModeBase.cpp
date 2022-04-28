// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	//ĳ���� �������Ʈ Ŭ���� ��������
	ConstructorHelpers::FClassFinder<APawn>
		Killer(TEXT("Blueprint'/Game/Blueprints/Killer/BP_Killer.BP_Killer_C'"));

	ConstructorHelpers::FClassFinder<APawn>
		Survivor(TEXT("Blueprint'/Game/Blueprints/Survivor/BP_Survivor.BP_Survivor_C'"));


	//������ ����UI ��������
	ConstructorHelpers::FClassFinder<UUserWidget> SurvivorMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/BP_MyMainHUD.BP_MyMainHUD_C'")); //_C �������ֱ�!
	//���θ� ����UI
	ConstructorHelpers::FClassFinder<UUserWidget> KillerMainHUD
	(TEXT("WidgetBlueprint'/Game/Blueprints/UI/InGameUI/Killer/BP_KillerMainHUD.BP_KillerMainHUD_C'")); //_C �������ֱ�!

	//���� ���� ��������
	EPLAYER_TYPE m_SelectType ;
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		m_SelectType = GI->GetSelectType();
		UE_LOG(LogTemp, Log, TEXT("m_SelectTypem_SelectType %i"), m_SelectType);
	}

	DefaultPawnClass = nullptr;

	//[1] ������ �ڵ�
	switch (m_SelectType)
	{
	case EPLAYER_TYPE::KILLER:
		if (Killer.Succeeded()) DefaultPawnClass = Killer.Class;
		if (KillerMainHUD.Succeeded()) m_MainHUDClass = KillerMainHUD.Class; //UI
		break;
	case EPLAYER_TYPE::SURVIVOR:
		if (Survivor.Succeeded()) DefaultPawnClass = Survivor.Class;
		if (SurvivorMainHUD.Succeeded()) m_MainHUDClass = SurvivorMainHUD.Class; //UI
		break;
	case EPLAYER_TYPE::RANDOM:
		int irandom = rand() % 2;
		if (irandom == 0) {
			//���θ�
			if (Killer.Succeeded()) DefaultPawnClass = Killer.Class;
			if (KillerMainHUD.Succeeded()) m_MainHUDClass = KillerMainHUD.Class;
			//���θ��� ����
			GI->SetSelectType(EPLAYER_TYPE::KILLER);
		}
		else {
			//������
			if (Survivor.Succeeded()) DefaultPawnClass = Survivor.Class;
			if (SurvivorMainHUD.Succeeded()) m_MainHUDClass = SurvivorMainHUD.Class;
			//�����ڷ� ���� ����
			GI->SetSelectType(EPLAYER_TYPE::SURVIVOR);
		}
		break;
	}
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//���� UI �����ϱ�
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
			m_MainHUD = Cast<UMyMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
			if (nullptr != m_MainHUD) m_MainHUD->AddToViewport();
		}
		else {
			m_KillerMainHUD = Cast<UMyKillerMainHUD>(CreateWidget(GetWorld(), m_MainHUDClass));
			if (nullptr != m_KillerMainHUD) m_KillerMainHUD->AddToViewport();
		}
		
	}

	// InputMode ����
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	FInputModeGameAndUI mode;

	Controller->SetInputMode(mode);
	Controller->bShowMouseCursor = true; // ������ ���콺 Ŀ���� ���̰� �Ѵ�.

}

void AMyPropGameModeBase::UpdatePlayHUD(float _CurHPRatio, float _CurSPRatio) {

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		if (GI->GetSelectType() == EPLAYER_TYPE::SURVIVOR) {
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
		else {

		}
	}
}