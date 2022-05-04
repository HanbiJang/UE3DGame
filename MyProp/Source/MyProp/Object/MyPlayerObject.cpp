// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObject.h"
#include "../Player/MyPlayerObjectPawn.h"
#include <math.h>

// Sets default values
AMyPlayerObject::AMyPlayerObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�Ž� �����
	//CreateDefaultSubObject ���ϴ� ������Ʈ�� ������	
	
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	//OnClicked.AddDynamic(this, &AMyPlayerObject::ChangePlayerMesh);

}

// Called when the game starts or when spawned
void AMyPlayerObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float MyGetDistance(FVector a, FVector b) {
	float res = sqrtf(powf(a.X - b.X, 2) + powf(a.Y - b.Y, 2) + powf(a.Z - b.Z, 2));
	return res;
}

//(Ŭ���� ���) �÷��̾�, ������Ʈ�� ������ ���� ��Ų��
void AMyPlayerObject::ChangePlayerToObject(ASurvivor* pCharacter)
{

	if (nullptr != pCharacter)
	{	
		if (pCharacter->m_state != EPLAYER_STATE::OBJECT) 
		{
			//�Ÿ��� ��ó�� ���� �����ϱ�
			//GetDistanceTo(this) : �� ������Ʈ�� �÷��̾� ĳ������ �Ÿ�
			//MyCharacter �ȿ� �ִ� ������Ʈ ���� �Լ��� ����, ������Ʈ�� �Žÿ� ������ ũ�⸦ �ѱ��
			if (pCharacter->GetDistanceTo(this) < 500) {
				pCharacter->ChangeToObject(/*m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D()*/);
			}
		}
		else 
		{ //�繰���� -> �繰���� 
			if (pCharacter->m_PlayerObject->GetStaticMesh() != m_Mesh->GetStaticMesh())
			{
				if (MyGetDistance(pCharacter->m_PlayerObject->GetRelativeLocation(), GetActorLocation()) < 400)
					pCharacter->ChangeObjectMesh(/*m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D()*/);
			}
		}		
	}
}




