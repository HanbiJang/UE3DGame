// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��Ŭ�� - ���� Ű
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);

}

void AKiller::Attack()
{
	//���� ���
}

void AKiller::BeginPlay() {
	Super::BeginPlay();

	//======�÷��̾� (�ʱ�) ������ ����======
	AMyCharacter* Character = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	const FKillerInfo* Info = nullptr;

	//������ ���̺� �������Ʈ���� ��������
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable(TEXT("DataTable'/Game/Blueprints/DT_Killer.DT_Killer'"));
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;
	}
	Info = m_PlayerTable->FindRow<FKillerInfo>(FName(TEXT("Killer1")), TEXT(""));

	SetInfo(*Info);
}
