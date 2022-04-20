// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"

AKiller::AKiller() {
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