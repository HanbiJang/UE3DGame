// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include "MyGameInstance.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>

AMyPropGameModeBase::AMyPropGameModeBase() {
	//ĳ���� �������Ʈ Ŭ���� ��������

	//[1] ������ �ڵ�
	ConstructorHelpers::FClassFinder<APawn>
		Survivor(TEXT("Blueprint'/Game/Blueprints/Survivor/BP_Survivor.BP_Survivor_C'"));
	if (Survivor.Succeeded())
	{
		//DefaultPawnClass = Survivor.Class;
	}

	//[2] ���θ� �ڵ�
	ConstructorHelpers::FClassFinder<APawn>
		Killer(TEXT("Blueprint'/Game/Blueprints/Killer/BP_Killer.BP_Killer_C'"));
	if (Killer.Succeeded())
	{
		DefaultPawnClass = Killer.Class;
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

	//======�÷��̾� (�ʱ�) ������ ����======
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FString TypeName_ = typeid(DefaultPawnClass).name();
	UE_LOG(LogTemp, Log, TEXT("%s"), *TypeName_);

	//[1] ������
	//if (GI != nullptr) {
	//	ASurvivor* Character = Cast<ASurvivor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//	Character->SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	//}

	//======�÷��̾� (�ʱ�) ������ ����======
	//[2] ų��
	if (GI != nullptr) {
		AKiller* Character = Cast<AKiller>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Character->SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
	}


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