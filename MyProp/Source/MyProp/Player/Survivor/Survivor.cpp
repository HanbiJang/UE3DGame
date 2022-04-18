// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
#include "../MyPlayerObjectPawn.h"

ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
{
	//======�÷��̾� (�ʱ�) ������ ����======
	const FSurvivorInfo* Info = nullptr;

	//������ ���̺� �������Ʈ���� ��������
	ConstructorHelpers::FObjectFinder<UDataTable> PlayerTable
	(TEXT("DataTable'/Game/Blueprints/DT_Survivor.DT_Survivor'"));
	
	if (PlayerTable.Succeeded())
	{
		m_PlayerTable = PlayerTable.Object;

		if (m_PlayerTable) {
			Info = m_PlayerTable->FindRow<FSurvivorInfo>(FName(TEXT("Survivor1")), TEXT(""));
			SetInfo(*Info);
		}
	}

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

	//======���̾� ���ſ� �繰 ������Ʈ �������Ʈ======
	//�����ͼ� ������Ű��
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(),
		TEXT("Blueprint'/Game/Blueprints/Objects/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));

	if (LoadBP)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PlayerObjectPawn = nullptr;

		m_PlayerObjectPawn = GetWorld()->SpawnActor<AMyPlayerObjectPawn>(LoadBP,
			FVector(0, 0, 0),//��ü�� �뷫�� ũ�� ���ϱ�
			FRotator(0, 0, 0),
			SpawnInfo);

		m_PlayerObjectPawn->SetPCharacter(this); //�ΰ��� �������

	}
}

void ASurvivor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//ü��, ���׹̳� �ǽð� ������Ʈ
	if (m_PrevHP != m_Info.fCurHP || m_PrevSP != m_Info.fCurSP)
	{
		// ���� ü�°� ���� ü���� �ٸ���.
		// HUD ����
		AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

		float HPRatio = m_Info.fCurHP / m_Info.fMaxHP;
		float MPRatio = m_Info.fCurSP / m_Info.fMaxSP;
		GM->UpdatePlayHUD(HPRatio, MPRatio);
	}

	// ���� ü���� ���������� ü������ ����
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;

}

void ASurvivor::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//�÷��̾� ���� ����
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //���� ��� ���� ��ġ ����

	//���� [�ΰ���] ������Ʈ �ٸ� ������ ġ��� && ���� ���۰� ���� �����ϱ�
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//���� ����
	m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(false);

	//��ü ��ġ�� �ΰ����� �ִ� ������ ����
	m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));

	//Ŭ���� ������Ʈ �޽��� ũ��� ���� ����
	m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//�Ž� ����
	m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);

	//���� �ű��...
	GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);

	//0.1�� �ڿ� ������Ʈ ���� �ѱ�
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FPhysicsTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);

	//���� ���� ���� ����
	//1.5�� �ڿ� ���� ����������
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);

	//ĳ���� ���� �ű��
	m_PlayerObjectPawn->SetInfo(m_Info);

}


