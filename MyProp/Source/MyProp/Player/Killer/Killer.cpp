// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"
#include <DrawDebugHelpers.h>
#include "../Survivor/Survivor.h" 
#include <MyProp/Player/MyEffectManager.h>

AKiller::AKiller():
	bAttackEnable(true),
	attackSpeed(2.f)
{
	//��Ƽ�÷��� - ���ø����̼� ����
	bReplicates = true;
	GetMesh()->SetIsReplicated(true); //���̷�Ż �Ž�

	//��Ÿ ����Ʈ
	ConstructorHelpers::FObjectFinder<UParticleSystem> AttackEffectAsset(
		TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit2.P_ky_hit2'"));
	if (AttackEffectAsset.Succeeded())
	{
		//m_AttackEffect->SetTemplate(AttackEffectAsset.Object);
		//m_AttackEffect->OnSystemFinished.AddDynamic(this, &AKiller::OnFinish); //��� �� ����
	}
}

void AKiller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ��Ŭ�� - ���� Ű
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AKiller::Attack);
	// QŰ - ���� ����
	PlayerInputComponent->BindAction(TEXT("Q"), EInputEvent::IE_Pressed, this, &AKiller::RangeAttack);
}

void AKiller::BeginPlay() {
	Super::BeginPlay();

	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//======�÷��̾� (�ʱ�) ������ ����======
	if (GI != nullptr) {
		SetInfo(*(GI->GetKillerInfo(TEXT("Killer1"))));
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
	ChangeState(EPLAYER_STATE::RANGEATTACK);
	RangeAttackEffect_Server();
}

void AKiller::RangeAttackEffect_Server_Implementation() {
	RangeAttackEffect_Multicast();
}

void AKiller::RangeAttackEffect_Multicast_Implementation() {
	
	//����Ʈ ���	
	FTransform trans(GetActorRotation(), GetActorLocation() - FVector(0, 0, (GetMesh()->Bounds.GetBox().GetSize().Z) / 2));
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::Q, trans, GetLevel());
}


void AKiller::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) {

}
void AKiller::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) {

}
void AKiller::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) {

}
