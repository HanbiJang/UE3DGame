// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Mode/MyPropGameModeBase.h>
#include "../Killer/Killer.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

//���ұ��� / �ߺ� ��� ���� ���� �ذ�
class Survivor_Move; 
class Survivor_Change;

/**
 * 
 */
UCLASS()
class MYPROP_API ASurvivor : public AMyCharacter
{
	GENERATED_BODY()

public:
	ASurvivor();

private:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);

	//�̵� (���)
	void Dash() override;
	void DashStop() override;

	//��ȣ�ۿ�
	void Interaction();

public:

	//ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;

	float m_PrevHP;
	float m_PrevSP;

	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }
	

	//ų�� ����
	TArray<AKiller*> arrKiller;

//���� ���
//ĳ������ ������Ʈ ��ȯ
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	//	AMyPlayerObjectPawn* m_PlayerObjectPawn; //�繰�� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_PlayerObject; //�繰�� ����ƽ �Ž� (���� ��� ����)

	FTimerHandle FPhysicsTimer; //���� �ۿ� ��/�� Ÿ�̸�
	FTimerHandle FChangeEnableTimer; //���� ���� �ð� ��/�� Ÿ�̸�
	void SetSimulatePhysicsTrue() { m_PlayerObject->SetSimulatePhysics(true); }

	//���� ���� ���� ��Ÿ���� ���� (���� ���� ��� ������)
	bool bChangeEnable;
	void SetbChangeEnableTrue() { bChangeEnable = true; }

	//���� �ʱ�ȭ ����, �ʱ�ȭ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;
	void ChangeToObject(UStaticMesh* mesh, FVector fscale); //���->������Ʈ ����
	void ChangeObjectMesh(UStaticMesh* mesh, FVector scale); //������Ʈ -> ��������Ʈ ����
	void ChangeToPlayer(); //������Ʈ->��� ����

	//���ſ� �̵�
	virtual void UpDown(float f) override;
	virtual void LeftRight(float f) override;
	virtual void Jump() override;
	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRunPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRoPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fJumpPower;
	//Jump ���� ����
	bool isGround;
	//2�� ���� ����
	int JumpCnt = 0;
	//���� ����� �� isGround�� true�� �����
protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	//�Ÿ����ϱ� �Լ�
	float MyGetDistance(FVector a, FVector b) {
		float res = sqrtf(powf(a.X - b.X, 2) + powf(a.Y - b.Y, 2) + powf(a.Z - b.Z, 2));
		return res;
	}

public:
	//����
	//�߰� ����ڵ� ���� ����
	USoundWave* SW_HeartBeat;
	//�߰� BGM ���� ����
	USoundWave* SW_Chase;
	//���� ������Ʈ
	UAudioComponent* AC_HeartBeat;
	UAudioComponent* AC_Chase;
};
