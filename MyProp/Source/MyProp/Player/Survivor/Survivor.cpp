// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
#include "../MyPlayerObjectPawn.h"


ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
{
	//���� ��������
	ConstructorHelpers::FObjectFinder<USoundWave> HeartBeatSoundAsset(TEXT("SoundWave'/Game/Music/HeartBeat_Fast.HeartBeat_Fast'"));

	if (HeartBeatSoundAsset.Succeeded())
		SW_HeartBeat = HeartBeatSoundAsset.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> ChaseSoundAsset(TEXT("SoundWave'/Game/Music/HorrorChaseMusic.HorrorChaseMusic'"));

	if (ChaseSoundAsset.Succeeded())
		SW_Chase = ChaseSoundAsset.Object;

	//����� ������Ʈ ����
	AC_HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_HeartBeat"));
	AC_Chase = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Chase"));
	AC_HeartBeat->AttachTo(GetMesh());
	AC_Chase->AttachTo(GetMesh());
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
		TEXT("Blueprint'/Game/Blueprints/Survivor/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));

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

	//ų�� ��������
	//���� ���� Ư�� Ŭ���� Actor�� ��������
	TArray<AActor*> arrActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKiller::StaticClass(), arrActor);
	for (int i = 0; i < arrActor.Num(); i++) {
		arrKiller.Add(Cast<AKiller>(arrActor[i])); //����ȯ
	}

	//���� �����ϱ�
	//AC_HeartBeat = UGameplayStatics::SpawnSound2D(this, SW_HeartBeat);
	AC_HeartBeat->SetSound(SW_HeartBeat);
	//AC_Chase = UGameplayStatics::SpawnSound2D(this, SW_Chase);
	AC_Chase->SetSound(SW_Chase);
	AC_HeartBeat->Stop();
	AC_Chase->Stop();
}

void ASurvivor::Dash()
{
	UE_LOG(LogTemp, Log, TEXT("Dasu!!!"));
	isDashPressed = true;

	if (GetInfo()->fCurSP > 10) {
		isDashEnable = true;
	}
}

void ASurvivor::DashStop()
{
	isDashPressed = false;
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

	// ���� ü��, ���¹̳ʸ� ���������� ü������ ����
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;

	//��� ���� ����)
	if (GetInfo()->fCurSP < 0.1) {
		isDashEnable = false;
	}

	//��� ��)
	//[1] ���¹̳� ����
	if (isDashEnable && isDashPressed && isMoving) {
		GetCharacterMovement()->MaxWalkSpeed = 1200.f; //�ӵ� ���� (���)	
		ChangeState(EPLAYER_STATE::DASH); //���� ��ȯ
		isDashed = true;

		GetInfo()->fCurSP -= 0.4f;
		if (GetInfo()->fCurSP < 0) GetInfo()->fCurSP = 0;
	}
	//[2] ��� �ƴ� ����) ���¹̳� ȸ��
	else{ 
		GetCharacterMovement()->MaxWalkSpeed = 600.f; //�ӵ� ���󺹱�
		isDashed = false;

		GetInfo()->fCurSP += 0.035f;
		if (GetInfo()->fCurSP > GetInfo()->fMaxSP) GetInfo()->fCurSP = GetInfo()->fMaxSP;
	}

	//======================================================================================

	//���θ��� �Ÿ� ���̿� ���� ����� ȿ�� & ȭ�� �׷��� (����) ���� ����
	for (int i = 0; i < arrKiller.Num(); i++) {

		if (this->GetDistanceTo(arrKiller[i]) < 500) {

			m_Cam->PostProcessSettings.VignetteIntensity += 0.05;
			if (m_Cam->PostProcessSettings.VignetteIntensity > 2) m_Cam->PostProcessSettings.VignetteIntensity = 2;

			m_Cam->PostProcessSettings.GrainIntensity += 0.05;
			if (m_Cam->PostProcessSettings.GrainIntensity > 1) m_Cam->PostProcessSettings.GrainIntensity = 1;

			m_Cam->PostProcessSettings.GrainJitter += 0.05;
			if (m_Cam->PostProcessSettings.GrainJitter > 1) m_Cam->PostProcessSettings.GrainJitter = 1;

			//���� ����
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this && !AC_HeartBeat->IsPlaying() && !AC_Chase->IsPlaying()) {
				UE_LOG(LogTemp, Log, TEXT("Killer Name!!! : %s"), *(arrKiller[i]->GetName()));
				UE_LOG(LogTemp, Log, TEXT("Killer distance!!! : %f"), GetDistanceTo(arrKiller[i]));

				AC_HeartBeat->Play();
				AC_Chase->Play();
			}
			
		}
		else {
			m_Cam->PostProcessSettings.VignetteIntensity -= 0.05;
			if (m_Cam->PostProcessSettings.VignetteIntensity <0 ) m_Cam->PostProcessSettings.VignetteIntensity = 0;

			m_Cam->PostProcessSettings.GrainIntensity -= 0.05;
			if (m_Cam->PostProcessSettings.GrainIntensity < 0) m_Cam->PostProcessSettings.GrainIntensity = 0;

			m_Cam->PostProcessSettings.GrainJitter -= 0.05;
			if (m_Cam->PostProcessSettings.GrainJitter < 0) m_Cam->PostProcessSettings.GrainJitter = 0;

			//���� ����
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this && AC_HeartBeat->IsPlaying() && AC_Chase->IsPlaying()) {
				AC_HeartBeat->Stop();
				AC_Chase->Stop();
			}
		}
	}

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


