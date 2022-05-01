// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
//���ұ���
#include "Survivor_Move.h"
#include "Survivor_Change.h"
#include "Multi/Survivor_Multi.h"

ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	fRunPower(5),
	bChangeEnable(false),
	fJumpPower(500),
	FRChange(0, 0, 0),
	fRoPower(0.1f),
	isGround(false),
	JumpCnt(0)
{

	//���ſ� �Ž� ����==================================================================
	m_PlayerObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_PlayerObject->SetupAttachment(RootComponent);
		//����
	m_PlayerObject->SetLinearDamping(0.15);
	m_PlayerObject->SetAngularDamping(3.0);
	m_PlayerObject->SetNotifyRigidBodyCollision(true);

	//���� ��������===================================================================
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

	//��Ƽ�÷��� ���ø����̼� ����
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //���̷�Ż �Ž�
	m_PlayerObject->SetIsReplicated(true); //�繰 �Ž�

}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);

	//�ΰ��� ����
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToPlayer);
}

void ASurvivor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	////������ ���� �������� ��
	//if (HasAuthority()) {

	//}

	if (m_state == EPLAYER_STATE::OBJECT && !isGround) {
		//������ �̿��� �� �浹 ������ ���� ���ϱ�
		float sizes = abs(HitNormal.Size() * FVector(0, 0, 1).Size()); //������ ũ��
		float dot = FVector::DotProduct(FVector(0, 0, 1), HitNormal); //������
		float angle = FMath::RadiansToDegrees(FMath::Acos(dot / sizes));
		if (angle <= 45) {
			isGround = true;
			JumpCnt = 0;
		}
	}
}

void ASurvivor::Interaction()
{
	//������, �� ��� ��ȣ�ۿ�
}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======�÷��̾� (�ʱ�) ������ ����==================================================
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	}

	//ų�� �������� =====================================================================
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
		if (GM != nullptr)
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

		if ((m_state != EPLAYER_STATE::OBJECT && this->GetDistanceTo(arrKiller[i]) < 500) 
			|| (m_state == EPLAYER_STATE::OBJECT && MyGetDistance(m_PlayerObject->GetRelativeLocation(), arrKiller[i]->GetActorLocation()) < 500)) {

			m_Cam->PostProcessSettings.VignetteIntensity += 0.05;
			if (m_Cam->PostProcessSettings.VignetteIntensity > 2) m_Cam->PostProcessSettings.VignetteIntensity = 2;

			m_Cam->PostProcessSettings.GrainIntensity += 0.05;
			if (m_Cam->PostProcessSettings.GrainIntensity > 1) m_Cam->PostProcessSettings.GrainIntensity = 1;

			m_Cam->PostProcessSettings.GrainJitter += 0.05;
			if (m_Cam->PostProcessSettings.GrainJitter > 1) m_Cam->PostProcessSettings.GrainJitter = 1;

			//���� ����
			if (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) == this && !AC_HeartBeat->IsPlaying() && !AC_Chase->IsPlaying()) {
				//UE_LOG(LogTemp, Log, TEXT("Killer Name!!! : %s"), *(arrKiller[i]->GetName()));
				//UE_LOG(LogTemp, Log, TEXT("Killer distance!!! : %f"), GetDistanceTo(arrKiller[i]));

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

	//�ױ�=======================================================================
	if (GetInfo()->fCurHP <= 0) ChangeState(EPLAYER_STATE::DEAD);
}

void ASurvivor::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
}

void ASurvivor::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
}

void ASurvivor::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{
}





