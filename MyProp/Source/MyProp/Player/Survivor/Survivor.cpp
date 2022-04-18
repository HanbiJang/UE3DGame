// Fill out your copyright notice in the Description page of Project Settings.


#include "Survivor.h"

ASurvivor::ASurvivor() {
	//======�÷��̾� (�ʱ�) ������ ����======
	const FServivorInfo* Info = nullptr;

	//������ ���̺� �������Ʈ���� ��������
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprints/DT_Survivor.DT_Survivor'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;
	}
	Info = m_PlayerTable->FindRow<FServivorInfo>(FName(TEXT("Survivor1")), TEXT(""));

	SetInfo(*Info);
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);
}

void ASurvivor::Interaction()
{
	//������, �� ��� ��ȣ�ۿ�
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();
}
