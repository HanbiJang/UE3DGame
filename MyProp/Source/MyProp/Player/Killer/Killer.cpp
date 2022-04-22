// Fill out your copyright notice in the Description page of Project Settings.


#include "Killer.h"
#include <DrawDebugHelpers.h>
#include "../Survivor/Survivor.h" 

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

void AKiller::BeginPlay() {
	Super::BeginPlay();
}

void AKiller::Attack()
{
	//���� ���
	ChangeState(EPLAYER_STATE::ATTACK);
	//AttackAction();
}

void AKiller::AttackAction() 
{
	//���� (�浹) Trace ä���� �̿��� ���� üũ	
	float fRadius = 80.f; //���� ũ��
	FVector attackPos = GetActorLocation() + FVector(0, 0, 50.f) + GetActorForwardVector()*100;

	TArray<FHitResult> arrHit; //�浹 ���
	FCollisionQueryParams param(NAME_None, false, this);
	//����
	GetWorld()->SweepMultiByChannel(arrHit, attackPos, attackPos, FQuat::Identity
		, ECC_GameTraceChannel3/*Attack Trace Channel*/
		, FCollisionShape::MakeSphere(fRadius), param);

	bool bSurvivor = false;
	if (arrHit.Num())
	{
		for (int i = 0; i < arrHit.Num(); i++) {
			if ( Cast<ASurvivor>(arrHit[i].GetActor()) ) { //cast ���н� null
				bSurvivor = true;
				break;
			}
		}

		if (bSurvivor) {
			//�����ڰ� �°��ϱ�
			for (int i = 0; i < arrHit.Num(); i++) {
				//AMyMonster* m_mon = Cast<AMyMonster>(arrHit[i].GetActor());
				//if (nullptr != m_mon) m_mon->HitFunc();
				UE_LOG(LogTemp, Log, TEXT("Attack Hit!!"));
					
			}
		}

		////�浹 �̸� ���
		//for (int i = 0; i < arrHit.Num(); ++i)
		//{
		//	UE_LOG(LogTemp, Log, TEXT("collision : %i"), arrHit.Num());
		//	UE_LOG(LogTemp, Log, TEXT("%s"), *arrHit[i].GetActor()->GetName());
		//}
	}


#ifdef ENABLE_DRAW_DEBUG //������ ������ Ȯ��
	FColor color;
	arrHit.Num() ? color = FColor::Red : color = FColor::Green;
	DrawDebugSphere(GetWorld(), attackPos, fRadius, 12, color, false, 2.5f);
#endif

}

void AKiller::RangeAttack() {
	ChangeState(EPLAYER_STATE::RANGEATTACK);
}

void AKiller::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult) {

}
void AKiller::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex) {

}
void AKiller::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit) {

}
