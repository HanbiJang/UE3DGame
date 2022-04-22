// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor_Change.h"

//�ΰ����� �繰�� ����
void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//�÷��̾� ���� ����
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //���� ��� ���� ��ġ ����

	//���� [�ΰ���] ������Ʈ �ٸ� ������ ġ��� && ���� ���۰� ���� �����ϱ�
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//���� ����
	//m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(false);
	m_PlayerObject->SetSimulatePhysics(false);

	//��ü ��ġ�� �ΰ����� �ִ� ������ ����
	//m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));
	m_PlayerObject->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));


	//Ŭ���� ������Ʈ �޽��� ũ��� ���� ����
	m_PlayerObject->SetRelativeScale3D(fscale);
	m_PlayerObject->SetStaticMesh(mesh);

	//���� �ű��...
	//ī�޶� �ű��
	m_Arm->DetachFromParent(true);
	m_Arm->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);

	//���� ������Ʈ �̵�
	AC_HeartBeat->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);
	AC_Chase->AttachToComponent(m_PlayerObject, FAttachmentTransformRules::KeepRelativeTransform);

	//0.1�� �ڿ� ������Ʈ ���� �ѱ�
	GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &ASurvivor::SetSimulatePhysicsTrue, 0.1f, false);

	//���� ���� ���� ���� 1.5�� �ڿ� ���� ����������
	GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);

	//���� �� ĳ���� ü�� �����
	//[�̱���]
}

//�繰���� �� �繰�� ����
void ASurvivor::ChangeObjectMesh(UStaticMesh* mesh, FVector scale)
{
	if (bChangeEnable) {
		bChangeEnable = false; //���ٺ��� ����

		//���� ����
		m_PlayerObject->SetSimulatePhysics(false);

		//�Ž��� ���� �ʱ�ȭ �ϱ�
		m_PlayerObject->SetAllPhysicsRotation(FRChange);
		//[�Ž�] �κ� �̵�
		m_PlayerObject->SetRelativeLocation(m_PlayerObject->GetRelativeLocation() + FVector(0, 0,
			m_PlayerObject->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//�Ž� ����
		m_PlayerObject->SetStaticMesh(mesh);
		//ũ�� ����
		m_PlayerObject->SetRelativeScale3D(scale);

		//0.1�� �ڿ� ���� �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &ASurvivor::SetSimulatePhysicsTrue, 0.1f, false);

		//1.5�� �ڿ� ���� ����������
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);
	}
}

//�繰���� ������� ����
void ASurvivor::ChangeToPlayer() {

	if (bChangeEnable) {
		bChangeEnable = false; //���ٺ��� ����

		//���� ����
		m_Arm->DetachFromParent(true);
		m_Arm->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		m_Arm->SetRelativeLocation(FVector(0, 0, 0));

		//���� ������Ʈ �̵�
		AC_HeartBeat->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		AC_Chase->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector originalPos = m_PlayerObject->GetRelativeLocation(); //�ΰ��� ���ư��� ��� ���� [�޽���] ��ġ ����

		m_PlayerObject->SetSimulatePhysics(false); //���� ����

		//�Ž� ����
		m_PlayerObject->SetAllPhysicsRotation(FRChange); //����
		m_PlayerObject->SetRelativeLocation(FVChange  //��ġ
			+ FVector(0, 0, m_PlayerObject->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//�ΰ����� ��ġ�� ������Ʈ�� �ִ� ���� ��ġ�� �����ϱ�
		SetActorLocation(originalPos);

		//1.5�� �ڿ� ���� ����������
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &ASurvivor::SetbChangeEnableTrue, 1.5f, false);

		//���� ��ȭ
		ChangeState(EPLAYER_STATE::IDLE);
	}
}