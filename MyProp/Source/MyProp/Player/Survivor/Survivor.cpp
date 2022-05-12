// Fill out your copyright notice in the Description page of Project Settings.

#include "Survivor.h"
//���ұ���
#include "Survivor_Move.h"
#include "Survivor_Change.h"
#include "Multi/Survivor_Multi.h"
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/Object/MyPlayerObject.h>


ASurvivor::ASurvivor() :
	FVChange(0, 0, 0),
	fRunPower(5),
	bChangeEnable(true),
	fJumpPower(500),
	FRChange(0, 0, 0),
	fRoPower(0.1f),
	isGround(false),
	JumpCnt(0),
	IsRepairEnable(false)
{

	//���ſ� �Ž� ����==================================================================
	m_PlayerObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_PlayerObject->SetupAttachment(RootComponent);
		//����
	m_PlayerObject->SetLinearDamping(0.15);
	m_PlayerObject->SetAngularDamping(3.0);
	m_PlayerObject->SetNotifyRigidBodyCollision(true);

	//���� ��������===================================================================
	ConstructorHelpers::FObjectFinder<USoundWave> HeartBeatSoundAsset(TEXT("SoundWave'xxx/Game/Music/HeartBeat_Fast.HeartBeat_Fast'"));

	if (HeartBeatSoundAsset.Succeeded())
		SW_HeartBeat = HeartBeatSoundAsset.Object;

	ConstructorHelpers::FObjectFinder<USoundWave> ChaseSoundAsset(TEXT("SoundWave'xxx/Game/Music/HorrorChaseMusic.HorrorChaseMusic'"));

	if (ChaseSoundAsset.Succeeded())
		SW_Chase = ChaseSoundAsset.Object;

	//����� ������Ʈ ����
	AC_HeartBeat = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_HeartBeat"));
	AC_Chase = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio_Chase"));
	AC_HeartBeat->AttachTo(GetMesh());
	AC_Chase->AttachTo(GetMesh());

	//��Ƽ�÷��� ���ø����̼� ����
	bReplicates = true;
	//GetMesh()->SetIsReplicated(true); //���̷�Ż �Ž�
	//m_PlayerObject->SetIsReplicated(true); //�繰 �Ž�

	//���͸��� ��������===========================================================
	//ĳ���Ͱ� ����ϴ� �Ž��Դϴ�
	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialHitAsset(TEXT("Material'/Game/Blueprints/MyMaterial/MT_MyHitRed.MT_MyHitRed'"));
	if (MaterialHitAsset.Succeeded())
		Setmat_hit_object(MaterialHitAsset.Object);

	ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialHitAsset_human(TEXT("MaterialInstanceConstant'/Game/Blueprints/MyMaterial/MI_ColorPalette_Hit.MI_ColorPalette_Hit'"));
	if (MaterialHitAsset_human.Succeeded())
		Setmat_hit_human(MaterialHitAsset_human.Object);
}

void ASurvivor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &ASurvivor::Dash); //������ ��� ����
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &ASurvivor::DashStop); //��� ����

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &ASurvivor::Interaction);

	//�繰�� ���� 
	PlayerInputComponent->BindAction(TEXT("Change"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToObject);

	//�ΰ��� ����
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &ASurvivor::ChangeToPlayer);
}

void ASurvivor::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{

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
	if (m_state != EPLAYER_STATE::OBJECT) {
		//���� ��ȯ (��� �ı�, ��� ����)
		if(IsRepairEnable)
			ChangeState(EPLAYER_STATE::MACHINE);
	}

}

void ASurvivor::BeginPlay() {
	Super::BeginPlay();

	//======�÷��̾� (�ʱ�) ������ ����==================================================
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GI != nullptr) {
		SetInfo(*(GI->GetSurvivorInfo(TEXT("Survivor1"))));
	}

	//�÷��̾� ��Ʈ�ѷ� ��������
	PC = Cast<AMyPlayerController>(GetController());

	//ų�� �������� =====================================================================
	//���� ���� Ư�� Ŭ���� Actor�� ��������
	TArray<AActor*> arrActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKiller::StaticClass(), arrActor);
	for (int i = 0; i < arrActor.Num(); i++) {
		arrKiller.Add(Cast<AKiller>(arrActor[i])); //����ȯ
		////ũ�� �����ϱ�
		//SetKillerScale_Server(Cast<AKiller>(arrActor[i]),1.2f);
		
	}

	//���� �����ϱ�
	//AC_HeartBeat = UGameplayStatics::SpawnSound2D(this, SW_HeartBeat);
	AC_HeartBeat->SetSound(SW_HeartBeat);
	//AC_Chase = UGameplayStatics::SpawnSound2D(this, SW_Chase);
	AC_Chase->SetSound(SW_Chase);
	AC_HeartBeat->Stop();
	AC_Chase->Stop();
}

////�ȵ�
//void ASurvivor::SetKillerScale_Server_Implementation(AKiller* killer,float f) {
//	SetKillerScale_Multicast(killer,f);
//}
//
//void ASurvivor::SetKillerScale_Multicast_Implementation(AKiller* killer,float f) {
//	if (killer) {
//		killer->GetMesh()->SetRelativeScale3D(FVector(f, f, f));
//		UE_LOG(LogTemp, Log, TEXT("set sets etset"));
//	}
//}

void ASurvivor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//����ĳ��Ʈ�� ���Ű��� ������Ʈ �Ǻ�======================================================
	SelectObject();

	//======================================================================================

	//���¹̳� ������Ʈ
	UpdateSP();

	//��� && ���¹̳� ��� && ȸ��
	UseSP();

	//======================================================================================

	//���θ��� �Ÿ� ���̿� ���� ����� ȿ�� & ȭ�� �׷��� (����) ���� ����
	ShowVinetting();

	//������ UI ������Ʈ=====================================================================
	if (PC) {
		int machineNum = PC->GetDoneMachineNum();
		//�̹��� �����ϱ�
		if (machineNum > 0)
			PC->GetMainHUD()->GetTimerHUD()->SetMachineImge_Done(machineNum - 1);
	}


	//�ױ�==================================================================================
	if (GetInfo()->fCurHP <= 0) {
		
		ChangeState(EPLAYER_STATE::DEAD);
		if (isObject) {
			ChangeToPlayer(); //�繰���̸� �ΰ������� �ٲ�� �ױ� 
		}

	}

	//�����Ⱑ ��� �����Ǹ�     ===========================================================
	if (PC) {
		if (PC->GetIsAllMachineRepaired()) {
			SetIsRepairEnable(false);
		}
	}

}

void ASurvivor::UpdateSP_Implementation() {

	UpdateSP_Client();
}

void ASurvivor::ShowVinetting_Implementation() {

	ShowVinetting_Client();
}

void ASurvivor::UpdateSP_Client_Implementation() {
	//ü��, ���׹̳� �ǽð� ������Ʈ
	if (m_PrevHP != m_Info.fCurHP || m_PrevSP != m_Info.fCurSP)
	{
		// ���� ü�°� ���� ü���� �ٸ���.
		// HUD ����
		float HPRatio = m_Info.fCurHP / m_Info.fMaxHP;
		float SPRatio = m_Info.fCurSP / m_Info.fMaxSP;

		if (PC != nullptr) PC->UpdatePlayHUD_Survivor(HPRatio, SPRatio, m_Info.fCurHP, m_Info.fMaxHP);

	}

	// ���� ü��, ���¹̳ʸ� ���������� ü������ ����
	m_PrevHP = m_Info.fCurHP;
	m_PrevSP = m_Info.fCurSP;
}


void ASurvivor::ShowVinetting_Client_Implementation(){
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
			if (m_Cam->PostProcessSettings.VignetteIntensity < 0) m_Cam->PostProcessSettings.VignetteIntensity = 0;

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


void ASurvivor::UseSP/*_Client_Implementation*/() {
	//��� ���� ����)
	if (GetInfo()->fCurSP < 0.1) {
		SetisDashEnable_Server(false);
		//isDashEnable = false;
	}
	//UE_LOG(LogTemp, Log, TEXT("isDashEnable : %i"), isDashEnable);
	//UE_LOG(LogTemp, Log, TEXT("isMoving : %i"), isMoving);
	//UE_LOG(LogTemp, Log, TEXT("isDashPressed : %i"), isDashPressed);
	//UE_LOG(LogTemp, Log, TEXT("isDashed : %i"), isDashed);

	//��� ��)
	//[1] ���¹̳� ����
	if (isDashEnable && isDashPressed && isMoving) {
		//GetCharacterMovement()->MaxWalkSpeed = 1200.f; //�ӵ� ���� (���)
		////DashSpeedUp_Server();
		ChangeState(EPLAYER_STATE::DASH); //���� ��ȯ
		//isDashed = true;
		SetisDashed_Server(true);

		GetInfo()->fCurSP -= 0.4f;
		if (GetInfo()->fCurSP < 0) GetInfo()->fCurSP = 0;
	}
	//[2] ��� �ƴ� ����) ���¹̳� ȸ��
	else {
		//GetCharacterMovement()->MaxWalkSpeed = 600.f; //�ӵ� ���󺹱�
		////DashSpeedDown_Server();

		//UE_LOG(LogTemp, Log, TEXT("isDashEnable : %i"), isDashEnable);
		//UE_LOG(LogTemp, Log, TEXT("isMoving : %i"), isMoving);
		//UE_LOG(LogTemp, Log, TEXT("isDashPressed : %i"), isDashPressed);
		//UE_LOG(LogTemp, Log, TEXT("isDashed : %i"), isDashed);

		SetisDashed_Server(false);
		//isDashed = false;

		GetInfo()->fCurSP += 0.035f;
		if (GetInfo()->fCurSP > GetInfo()->fMaxSP) GetInfo()->fCurSP = GetInfo()->fMaxSP;
	}

	//�뽬�����϶� �ӷ� ����
	if (isDashed) {
		if (GetCharacterMovement()->MaxWalkSpeed != 1200.f) DashSpeedUp_Server();
	}
	else {
		if (GetCharacterMovement()->MaxWalkSpeed != 600.f) DashSpeedDown_Server();
	}

}

void ASurvivor::DashSpeedUp_Server_Implementation(){
	DashSpeedUp_Multicast();
}
void ASurvivor::DashSpeedUp_Multicast_Implementation() {
	GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	UE_LOG(LogTemp, Log, TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ASurvivor::DashSpeedDown_Server_Implementation(){
	DashSpeedDown_Multicast();
}
void ASurvivor::DashSpeedDown_Multicast_Implementation(){
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	UE_LOG(LogTemp, Log, TEXT("MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
}

void ASurvivor::TurnRed() {

	if (isObject) { //�繰
		Setmat_original(m_PlayerObject->GetMaterial(0));
		//�������� ���׸����� �����ϰ� ���� ���͸���� �ٲ�
		if (Getmat_hit_object())
			m_PlayerObject->SetMaterial(0, Getmat_hit_object());
	}
	else { //���
		Setmat_original(GetMesh()->GetMaterial(0));
		//�������� ���׸����� �����ϰ� ���� ���͸���� �ٲ�
		if (Getmat_hit_human()) {
			GetMesh()->SetMaterial(0, Getmat_hit_human());
		}
			
	}

}
void ASurvivor::TurnOriginalColor() {

	if (Getmat_original()) {
		if (isObject) {
			m_PlayerObject->SetMaterial(0, Getmat_original());
		}
		else {
			GetMesh()->SetMaterial(0, Getmat_original());
		}
		Setmat_original(nullptr);
	}

}


void ASurvivor::HitColorReaction_Server_Implementation() //�������� �ٽ� ���ƿ�
{
	HitColorReaction_Multicast();
}
void ASurvivor::HitColorReaction_Multicast_Implementation() //�������� �ٽ� ���ƿ�
{
	TurnRed();
	GetWorld()->GetTimerManager().SetTimer(FHitRedTimer, this, &ASurvivor::TurnOriginalColor, 0.5f, false);
}

void ASurvivor::TurnMove() {
	if ((!isDashed && !isJumping) && m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
}

//=======================

void ASurvivor::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
}

void ASurvivor::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
}

void ASurvivor::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{
}
