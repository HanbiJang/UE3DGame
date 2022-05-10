// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"
#include <DrawDebugHelpers.h>
#include "../Survivor/Survivor.h" 
#include <MyProp/Player/Effect/MyEffectManager.h>
#include <MyProp/Player/Killer/Projectile/KillerRCProjectile.h>
#include <MyProp/Controller/MyPlayerController.h>

AKiller::AKiller():
	bAttackEnable(true),
	attackSpeed(2.f), //��Ÿ ��Ÿ��
	bRangeAttackEnable(true),
	bRCAttackEnable(true),
	bEAttackEnable(true)

{
	//��Ƽ�÷��� - ���ø����̼� ����
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //���̷�Ż �Ž�

	//Į��� �ְ� �ϱ�
	m_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	m_Weapon->SetupAttachment(GetMesh(), TEXT("hand_r_weapon"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> BasicWeapon(TEXT("StaticMesh'/Game/PolygonPrototype/Meshes/Props/SM_Prop_Knife_01.SM_Prop_Knife_01'"));

	if (BasicWeapon.Succeeded()) {
		BasicWeaponMesh = BasicWeapon.Object;
		m_Weapon->SetStaticMesh(BasicWeaponMesh);
	}	

	//����ü �������Ʈ ���� 
	ConstructorHelpers::FClassFinder<AMyProjectile> BPProjectile(
		TEXT("Blueprint'/Game/Blueprints/Killer/Projectile/BP_KillerRCAttackProjectile.BP_KillerRCAttackProjectile_C'"));

	if (BPProjectile.Succeeded()) {
		m_RCAttackProjectile = BPProjectile.Class;
	}

	//�ҹ��
	HeadBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HeadBox"));
	HeadBox->SetupAttachment(GetMesh(), TEXT("head"));

}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��Ŭ�� - ���� Ű
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// QŰ - ���� ����
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
	// EŰ -  ����
	PlayerInputComponent->BindAction(TEXT("E"), EInputEvent::IE_Pressed, this, &AKiller::EAttack);
	// ��Ŭ�� - ��ô ����
	PlayerInputComponent->BindAction(TEXT("RCAttack"), EInputEvent::IE_Pressed, this, &AKiller::RCAttack);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//======�÷��̾� (�ʱ�) ������ ����======
	if (GI != nullptr) {
		SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
	}

	PC = Cast<AMyPlayerController>(this->GetInstigatorController());

	//======�ҽ���======
	//GetBox()->OnComponentHit.AddDynamic(this, &AMyProjectile::OnHit);
	HeadBox->OnComponentBeginOverlap.AddDynamic(this, &AKiller::OnBeginOverlap);
}

void AKiller::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if(HasAuthority())
	UpdateUI_Server();

	//��Ÿ�� �ð� ���
	if (!bRangeAttackEnable) {
		//Clamp: 0�� maxHP �� ���̿��� ������
		m_Info.fCurQLeftTime = FMath::Clamp(m_Info.fCurQLeftTime-DeltaTime, 0.f, m_Info.rangeAttackSpeed);		
	}

	if (!bEAttackEnable) {
		m_Info.fCurELeftTime = FMath::Clamp(m_Info.fCurELeftTime - DeltaTime, 0.f, m_Info.EAttackSpeed);
	}

	if (!bRCAttackEnable){
		m_Info.fCurRCLeftTime = FMath::Clamp(m_Info.fCurRCLeftTime - DeltaTime, 0.f, m_Info.rCAttackSpeed);
	}

}

void AKiller::Attack()
{
	//���� ���
	if (bAttackEnable) {
		ChangeState(EPLAYER_STATE::ATTACK);
		bAttackEnable = false;

		//attackSpeed�� �ڿ� Timer �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FAttackTimer, this, &AKiller::SetAttackEnable, attackSpeed, false);		
	}	
}

void AKiller::AttackAction() 
{
	//���� (�浹) Trace ä���� �̿��� ���� üũ	
	float fRadius = 80.f; //���� ũ��
	FVector attackPos = GetActorLocation() + FVector(0, 0, 50.f) + GetActorForwardVector()*100;
	ASurvivor* survivor;

	TArray<FHitResult> arrHit; //�浹 ���
	FCollisionQueryParams param(NAME_None, false, this);
	//����
	GetWorld()->SweepMultiByChannel(arrHit, attackPos, attackPos, FQuat::Identity
		, ECC_GameTraceChannel3/*Attack Trace Channel*/
		, FCollisionShape::MakeSphere(fRadius), param);

	//bool bSurvivor = false;
	if (arrHit.Num())
	{
		for (int i = 0; i < arrHit.Num(); i++) {
			survivor = Cast<ASurvivor>(arrHit[i].GetActor());
			
			//�����ڵ� �ƴϵ�, ���� ������ ����Ʈ ǥ���ϱ�
			FTransform trans(GetActorRotation(), arrHit[i].Location);
			UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::ATTACK, trans, GetLevel());

			if (survivor != nullptr) { //cast ���н� null
				//�����ڰ� �°��ϱ�
				UGameplayStatics::ApplyDamage(survivor,30.f,NULL,GetOwner(),NULL);

				break;
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG //������ ������ Ȯ��
	//FColor color;
	//arrHit.Num() ? color = FColor::Red : color = FColor::Green;
	//DrawDebugSphere(GetWorld(), attackPos, fRadius, 12, color, false, 2.5f);
#endif

}

void AKiller::RangeAttack() {
	//���� ���
	if (bRangeAttackEnable) {
		ChangeState(EPLAYER_STATE::RANGEATTACK);
		bRangeAttackEnable = false;

		RangeAttackEffect_Server(); //���� ����Ʈ

		//��Ÿ�� UI �ʱ�ȭ
		m_Info.fCurQLeftTime = m_Info.rangeAttackSpeed;
		//rangeAttackSpeed�� �ڿ� Timer �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FRangeAttackTimer, this, &AKiller::SetRangeAttackEnable, m_Info.rangeAttackSpeed, false);
	}	
}

void AKiller::RangeAttackAction() {

	//���� (�浹) Trace ä���� �̿��� ���� üũ	
	float fRadius = 300.f; //���� ũ��
	FVector attackPos = GetActorLocation();
	ASurvivor* survivor;

	TArray<FHitResult> arrHit; //�浹 ���
	FCollisionQueryParams param(NAME_None, false, this);
	//����
	GetWorld()->SweepMultiByChannel(arrHit, attackPos, attackPos, FQuat::Identity
		, ECC_GameTraceChannel3/*Attack Trace Channel*/
		, FCollisionShape::MakeSphere(fRadius), param);

	if (arrHit.Num())
	{
		for (int i = 0; i < arrHit.Num(); i++) {
			survivor = Cast<ASurvivor>(arrHit[i].GetActor());

			//�����ڵ� �ƴϵ�, ���� ������ ����Ʈ ǥ���ϱ�
			FTransform trans(GetActorRotation(), arrHit[i].ImpactPoint);
			UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::ATTACK, trans, GetLevel());

			UE_LOG(LogTemp, Log, TEXT("%s"), *(arrHit[i].GetActor()->GetName()) );

			if (survivor != nullptr) { //cast ���н� null
				//�����ڰ� �°��ϱ�
				UGameplayStatics::ApplyDamage(survivor, 5.f, NULL, GetOwner(), NULL);
				break;
			}
		}
	}

#ifdef ENABLE_DRAW_DEBUG //������ ������ Ȯ��
	FColor color;
	arrHit.Num() ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), attackPos, fRadius, 12, color, false, 2.5f);
#endif

}

void AKiller::RCAttack() {
	//���� ���
	if (bRCAttackEnable) {
		ChangeState(EPLAYER_STATE::RCATTACK);
		bRCAttackEnable = false;

		//��Ÿ�� UI �ʱ�ȭ
		m_Info.fCurRCLeftTime = m_Info.rCAttackSpeed;
		//attackSpeed�� �ڿ� Timer �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FRCAttackTimer, this, &AKiller::SetRCAttackEnable, m_Info.rCAttackSpeed, false);
	}
}
void AKiller::RCAttackAction() {
	//���� ������
	//����ü ��ȯ, ����ü�� ���ư��� ������ �±� ����

	// ����ü ����
	FActorSpawnParameters SpawnParam = {};
	SpawnParam.OverrideLevel = GetLevel();
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParam.bDeferConstruction = true;

	//�������Ʈ�� ����ü ����
	if (m_RCAttackProjectile != nullptr) {
		AKillerRCProjectile* pRCAttackProjectile = GetWorld()->SpawnActor<AKillerRCProjectile>(m_RCAttackProjectile
			, GetActorLocation() + GetActorForwardVector() * 50.f
			, GetActorForwardVector().Rotation()
			, SpawnParam);
		if (pRCAttackProjectile != nullptr) {
			// begin play ȣ��
			pRCAttackProjectile->FinishSpawning(pRCAttackProjectile->GetTransform());
		}
	}

}

void AKiller::RangeAttackEffect_Server_Implementation() {
	RangeAttackEffect_Multicast();
}

void AKiller::RangeAttackEffect_Multicast_Implementation() {
	
	//����Ʈ ���	
	FTransform trans(GetActorRotation(), GetActorLocation() - FVector(0, 0, (GetMesh()->Bounds.GetBox().GetSize().Z) / 2));
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::Q, trans, GetLevel());
}

void AKiller::EAttack() {
	//���� ���
	if (bEAttackEnable) {
		ChangeState(EPLAYER_STATE::EATTACK);
		bEAttackEnable = false;

		//��Ÿ�� UI �ʱ�ȭ
		m_Info.fCurELeftTime = m_Info.EAttackSpeed;
		//attackSpeed�� �ڿ� Timer �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FEAttackTimer, this, &AKiller::SetEAttackEnable, m_Info.EAttackSpeed, false);
	}
}
void AKiller::EAttackAction() {

}

void AKiller::UpdateUI_Server_Implementation() {
	//ü��, ���׹̳� �ǽð� ������Ʈ
	if (fPrevRCLeftTime != m_Info.fCurRCLeftTime || fPrevELeftTime != m_Info.fCurELeftTime
		|| fPrevQLeftTime != m_Info.fCurQLeftTime)
	{
		// ���� ��Ÿ���� ���� ��Ÿ�Ӱ� �ٸ���
		// HUD ����
		//AMyPlayerController* PC = Cast<AMyPlayerController>(this->GetInstigatorController());

		float CurQSkillRatio = m_Info.fCurQLeftTime / m_Info.rangeAttackSpeed;
		float CurESkillRatio = m_Info.fCurELeftTime / m_Info.EAttackSpeed;
		float CurRCSkillRatio = m_Info.fCurRCLeftTime / m_Info.rCAttackSpeed;

		if (PC != nullptr) PC->UpdatePlayHUD_Killer(CurQSkillRatio, CurESkillRatio, CurRCSkillRatio,
			m_Info.fCurQLeftTime, m_Info.fCurELeftTime, m_Info.fCurRCLeftTime);
	}

	// ���� ü��, ���¹̳ʸ� ���������� ü������ ����
	fPrevQLeftTime = m_Info.fCurQLeftTime;
	fPrevELeftTime = m_Info.fCurELeftTime;
	fPrevRCLeftTime = m_Info.fCurRCLeftTime;

}

void AKiller::TurnMove() {
	if ((!isDashed && !isJumping)) ChangeState(EPLAYER_STATE::MOVE);
}

//====================================================================
void AKiller::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) {
	UE_LOG(LogTemp, Log, TEXT(" AKiller OnBeginOverlap OnBeginOverlap OnBeginOverlap "));

	//�ڽ� ������ �̺�Ʈ -> change Hit
	ASurvivor* sur = Cast<ASurvivor>(_OtherActor);
	if (sur)
		ChangeState(EPLAYER_STATE::HIT);

}
void AKiller::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) {

}
void AKiller::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) {
}
