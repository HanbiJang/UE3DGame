// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <MyProp/Player/MyInfo.h>

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Killer.generated.h"


class ASurvivor;
/**
 * 
 */
UCLASS()
class MYPROP_API AKiller : public AMyCharacter
{
	GENERATED_BODY()

public:
		AKiller();

private:
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void BeginPlay() override;

	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);


public:
	//ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FKillerInfo m_Info;

	float m_PrevSP;

	FKillerInfo* GetInfo() { return &m_Info; }
	void SetInfo(FKillerInfo new_Info) { m_Info = new_Info; }
	
	//Q ����

public:
	//����
	UStaticMeshComponent* m_Weapon;
	UStaticMesh* BasicWeaponMesh;

	void RangeAttack();
	void RangeAttackAction(); //���� ������
	void Attack();
	void AttackAction();
	void RCAttack();
	void RCAttackAction(); //���� ������

private:
	//���� ��Ÿ�� ����
	//[1] ��Ÿ
	bool bAttackEnable;
	FTimerHandle FAttackTimer; //���� Ÿ�̸�
	void SetAttackEnable(){ bAttackEnable = true; }
	float attackSpeed;

	//[2] Q����
	bool bRangeAttackEnable;
	FTimerHandle FRangeAttackTimer; //���� Ÿ�̸�
	void SetRangeAttackEnable() { bRangeAttackEnable = true; }
	float rangeAttackSpeed;

	//[3] ��Ŭ�� ����
	bool bRCAttackEnable;
	FTimerHandle FRCAttackTimer; //���� Ÿ�̸�
	void SetRCAttackEnable() { bRCAttackEnable = true; }
	float rCAttackSpeed;

	//attack effect
	UParticleSystem* m_AttackEffect;

private:
	UFUNCTION(Reliable, Server)
		void RangeAttackEffect_Server();
	UFUNCTION(Reliable, NetMulticast)
		void RangeAttackEffect_Multicast();
};

