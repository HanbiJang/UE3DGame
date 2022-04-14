// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObject.h"
//#include "../Player/MyPlayerObjectPawn.h"
#include "../Player/MyCharacterObject.h"

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

void AMyPlayerObject::ChangePlayerToObject()
{
	//(Ŭ���� ���) �÷��̾�, ������Ʈ�� ������ ���� ��Ų��

	//[�÷��̾� -> ������Ʈ] ���忡�� �÷��̾� pawn �� �༮�� ��������
	AMyCharacter* pCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (nullptr != pCharacter)
	{		
		//MyCharacter �ȿ� �ִ� ������Ʈ ���� �Լ��� ����, ������Ʈ�� �Žÿ� ������ ũ�⸦ �ѱ��
		pCharacter->ChangeToObject(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
	}
	else {
		//[������Ʈ -> �÷��̾�]
		//UE_LOG(LogTemp, Log, TEXT("null!"));

		//AMyPlayerObjectPawn* pPlayerObject = Cast<AMyPlayerObjectPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		AMyCharacterObject* pPlayerObject = Cast<AMyCharacterObject>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		
		if (nullptr != pPlayerObject && pPlayerObject->m_ObjectMesh->GetStaticMesh() != m_Mesh->GetStaticMesh()) {
			pPlayerObject->ChangeObjectMesh(m_Mesh->GetStaticMesh(), m_Mesh->GetRelativeScale3D());
		}
	}

}



