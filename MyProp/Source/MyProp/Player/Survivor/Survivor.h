// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Mode/MyPropGameModeBase.h>
#include "../Killer/Killer.h"
#include "Materials/MaterialInterface.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

//���ұ��� / �ߺ� ��� ���� ���� �ذ�
class Survivor_Move; 
class Survivor_Change;
class Survivor_Multi;
class AMyPlayerObject;

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
	//���ø����̼� ����
	UPROPERTY(/*ReplicatedUsing = OnRep_Info,*/replicated,EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;

	UPROPERTY(replicated)
	float m_PrevHP;
	UPROPERTY(replicated)
	float m_PrevSP;

	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }

	AMyPlayerController* PC; //�÷��̾� ��Ʈ�ѷ�
	
	//ų�� ����
	TArray<AKiller*> arrKiller;

//���� ���
//ĳ������ ������Ʈ ��ȯ
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_PlayerObject; //�繰�� ����ƽ �Ž� (���� ��� ����)

	UPROPERTY(replicated)
	FTimerHandle FPhysicsTimer; //���� �ۿ� ��/�� Ÿ�̸�
	UPROPERTY(replicated)
	FTimerHandle FChangeEnableTimer; //���� ���� �ð� ��/�� Ÿ�̸�
	void SetSimulatePhysicsTrue() { m_PlayerObject->SetSimulatePhysics(true); }

	//���� ���� ���� ��Ÿ���� ���� (���� ���� ��� ������)
	UPROPERTY(replicated)
	bool bChangeEnable;
	void SetbChangeEnableTrue() { bChangeEnable = true; }

	//3��Ī ���� (�繰 ���� ����ĳ��Ʈ)
	void SelectObject();
	//�繰 �ܰ��� �׸��� ���
	UFUNCTION(Reliable, Client)
		void DrawOutLine();

	bool isChangableObject = false;
	bool isChangableObjectSelected = false;
	AMyPlayerObject* pPlayerObject; //new
	AMyPlayerObject* pPlayerObject_old; //old

	//���� �ʱ�ȭ ����, �ʱ�ȭ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	UFUNCTION(Reliable, Server)
	void ChangeToObject(/*UStaticMesh* myMesh, FVector fscale*/); //���->������Ʈ ����
	UFUNCTION(Reliable, NetMulticast)
	void ChangeToObject_Multicast(/*UStaticMesh* myMesh, FVector fscale*/); //���->������Ʈ ����

	UFUNCTION(Reliable, Server)
	void ChangeObjectMesh(/*UStaticMesh* myMesh, FVector scale*/); //������Ʈ -> ��������Ʈ ����
	UFUNCTION(Reliable, NetMulticast)
	void ChangeObjectMesh_Multicast(/*UStaticMesh* myMesh, FVector scale*/); //������Ʈ -> ��������Ʈ ����
	
	UFUNCTION(Reliable, Server)
	void ChangeToPlayer(); //������Ʈ->��� ����
	UFUNCTION(Reliable, NetMulticast)
	void ChangeToPlayer_Multicast(); //������Ʈ -> ��������Ʈ ����

	//���ſ� �̵�
	virtual void UpDown(float f) override;
	virtual void LeftRight(float f) override;
	virtual void MyJump() override;

	//������Ʈ�� �̵� (���������� ����)
	UFUNCTION(Reliable, Server)
		virtual void UpDown_Server(float f);
	UFUNCTION(Reliable, Server)
		virtual void LeftRight_Server(float f);
	UFUNCTION(Reliable, Server)
		virtual void MyJump_Server();

	//������Ʈ�� �̵� (����, Ŭ���̾�Ʈ ���� ����)
	UFUNCTION(Reliable, NetMulticast)
	virtual void UpDown_Multicast(float f);
	UFUNCTION(Reliable, NetMulticast)
	virtual void LeftRight_Multicast(float f);
	UFUNCTION(Reliable, NetMulticast)
	virtual void MyJump_Multicast();

	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRunPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRoPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fJumpPower;
	
	//Jump ���� ����
	UPROPERTY(replicated)
	bool isGround;

	//2�� ���� ����
	UPROPERTY(replicated)
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AC_HeartBeat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* AC_Chase;

	//��Ƽ�÷���==============================
	//ĳ���� Info�� �����ϱ�
	UFUNCTION()
		void OnRep_Info();

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Response to health being updated. 
	Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnInfoUpdate();

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float healthValue);

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	//����
	UFUNCTION(Reliable, Server)
		void SetBodyLocation();

	UFUNCTION(Reliable, NetMulticast)
		void SetBodyLocation_Multicast();

	//���¹̳� ������Ʈ
	UFUNCTION(Reliable, Server)
		void UpdateSP();

	//UFUNCTION(Reliable, Server)
	//	void UseSP();

	//����� ���� ȿ��
	UFUNCTION(Reliable, Server)
		void ShowVinetting();

	UFUNCTION(Reliable, Client)
		void UpdateSP_Client();

	/*UFUNCTION(Reliable, Client)*/
		void UseSP/*_Client*/();

	UFUNCTION(Reliable, Server)
		void DashSpeedUp_Server();
	UFUNCTION(Reliable, NetMulticast)
		void DashSpeedUp_Multicast();

	UFUNCTION(Reliable, Server)
		void DashSpeedDown_Server();
	UFUNCTION(Reliable, NetMulticast)
		void DashSpeedDown_Multicast();

	//����� ȿ��
	UFUNCTION(Reliable, Client)
		void ShowVinetting_Client();

	//�±� ȿ��
	//���� ��������!
	void TurnRed();
	void TurnOriginalColor();

	UFUNCTION(Reliable, Server)
		void HitColorReaction_Server(); //�������� �ٽ� ���ƿ�
	UFUNCTION(Reliable, NetMulticast)
		void HitColorReaction_Multicast(); //�������� �ٽ� ���ƿ�


	UMaterialInterface* mat_hit_object;
	UMaterialInterface* mat_hit_human;
	UMaterialInterface* mat_original;
	UPROPERTY(replicated)
		FTimerHandle FHitRedTimer; //���� �ڿ� ��Ʈ �Žÿ��� �������� �Ž÷� ���ƿ��� Ÿ�̸�

	void TurnMove() override;
public:
	//������ ����
	bool IsRepairEnable;
	void SetIsRepairEnable(bool value) { IsRepairEnable = value; };

	////ų�� ũ��
	//UFUNCTION(Reliable, Server)
	//	void SetKillerScale_Server(AKiller* killer,float f);
	//UFUNCTION(Reliable, NetMulticast)
	//	void SetKillerScale_Multicast(AKiller* killer,float f);
	
};
