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
	//������ ���̺��� ��������

	SetInfo(*Info);
}
