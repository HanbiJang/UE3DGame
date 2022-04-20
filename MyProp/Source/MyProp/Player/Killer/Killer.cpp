// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

AKiller::AKiller() {

	//======�÷��̾� (�ʱ�) ������ ����======
	const FKillerInfo* Info = nullptr;

	//������ ���̺� �������Ʈ���� ��������
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprints/Killer/DT_Killer.DT_Killer'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;

		if (m_PlayerTable) {
			Info = m_PlayerTable->FindRow<FKillerInfo>(FName(TEXT("Killer1")), TEXT(""));
			SetInfo(*Info);
		}

	}

}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��Ŭ�� - ���� Ű
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// QŰ - ���� ����
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
}

void AKiller::Attack()
{
	//���� ���
	ChangeState(EPLAYER_STATE::ATTACK);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();
}

void AKiller::RangeAttack() {
	ChangeState(EPLAYER_STATE::RANGEATTACK);
}