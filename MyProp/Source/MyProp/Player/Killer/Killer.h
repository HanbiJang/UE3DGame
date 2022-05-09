// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <MyProp/Player/MyInfo.h>
#include <MyProp/Player/Killer/MyProjectile.h>

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Killer.generated.h"

class AMyPlayerController;
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
	void Tick(float DeltaTime) override;
	void BeginPlay() override;

	UFUNCTION() //HeadBox�� �Ҵ�Ǵ� �̺�Ʈ
		void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);


public:
	//ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FKillerInfo m_Info;

	//float m_PrevSP;

	FKillerInfo* GetInfo() { return &m_Info; }
	void SetInfo(FKillerInfo new_Info) { m_Info = new_Info; }

public:
	//����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Weapon;
	UStaticMesh* BasicWeaponMesh;

	void RangeAttack();
	void RangeAttackAction(); //���� ������
	void Attack();
	void AttackAction();
	void RCAttack();
	void RCAttackAction(); //���� ������
	void EAttack();
	void EAttackAction(); //���� ������

private:
	//���� ��Ÿ�� ����
	AMyPlayerController* PC;

	//[1] ��Ÿ
	bool bAttackEnable;
	FTimerHandle FAttackTimer; //���� Ÿ�̸�
	void SetAttackEnable(){ bAttackEnable = true; }
	float attackSpeed;

	//[2] Q����
	bool bRangeAttackEnable;
	FTimerHandle FRangeAttackTimer; //���� Ÿ�̸�
	void SetRangeAttackEnable() { bRangeAttackEnable = true; }
	//float fCurQLeftTime; //���� �ð�
	float fPrevQLeftTime; //���� �ð� (����)

	//[3] ��Ŭ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyProjectile> m_RCAttackProjectile; //��Ŭ���� ��ƼŬ
	bool bRCAttackEnable;
	FTimerHandle FRCAttackTimer; //���� Ÿ�̸�
	void SetRCAttackEnable() { bRCAttackEnable = true; }
	//float fCurRCLeftTime; //���� �ð�
	float fPrevRCLeftTime; //���� �ð� (����)

	//[4] EŬ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AMyProjectile> m_EAttackProjectile; //��Ŭ���� ��ƼŬ
	bool bEAttackEnable;
	FTimerHandle FEAttackTimer; //���� Ÿ�̸�
	void SetEAttackEnable() { bEAttackEnable = true; }
	//float fCurELeftTime; //���� �ð�
	float fPrevELeftTime; //���� �ð� (����)

	//attack effect
	UParticleSystem* m_AttackEffect;

private:
	UFUNCTION(Reliable, Server)
		void RangeAttackEffect_Server();
	UFUNCTION(Reliable, NetMulticast)
		void RangeAttackEffect_Multicast();

	//UI ������Ʈ ============================
	UFUNCTION(Reliable, Server)
		void UpdateUI_Server();

	//(��) �ǰݽ�====================================================
	void TurnMove() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* HeadBox;
};

