// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyInfo.h" //ĳ���� ���� 
#include <EngineMinimal.h>
#include "MyCharacterState.h"
#include "../MyGameInstance.h"

#include "Kismet/KismetMathLibrary.h"

//��Ƽ�÷���
#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UMyAnimInstance; //�ߺ� ��� ���� ���� �ذ�
class AMyPlayerObjectPawn; //�ߺ� ��� ���� ���� �ذ�
class ASurvivor;

UCLASS()
class MYPROP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	/// <summary>
	/// 1. ĳ���� �Ž� ����,
	/// </summary>
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input	
	/// <summary>
	/// �Է�Ű�� �Լ��� �����ϱ�
	/// </summary>
	/// <param name="PlayerInputComponent"></param>
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//���
	virtual void Dash();
	virtual void DashStop();

	UPROPERTY(replicated)
		bool isMoving;

	UPROPERTY(replicated)
		bool isJumping;

	UPROPERTY(replicated)
		bool isDashed;
	UPROPERTY(replicated)
		bool isDashPressed;
	UPROPERTY(replicated)
		bool isDashEnable;

	//�� �ٲٱ�(����)
	UFUNCTION(Reliable, Server)
		void SetisDashEnable_Server(bool value);

	//�� �ٲٱ�(��Ƽĳ��Ʈ)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashEnable_Multicast(bool value);

	//�� �ٲٱ�(����)
	UFUNCTION(Reliable, Server)
		void SetisDashPressed_Server(bool value);

	//�� �ٲٱ�(��Ƽĳ��Ʈ)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashPressed_Multicast(bool value);

	//�� �ٲٱ�(����)
	UFUNCTION(Reliable, Server)
		void SetisDashed_Server(bool value);

	//�� �ٲٱ�(��Ƽĳ��Ʈ)
	UFUNCTION(Reliable, NetMulticast)
		void SetisDashed_Multicast(bool value);

	//�� �ٲٱ�(����)
	UFUNCTION(Reliable, Server)
		void SetisMoving_Server(bool value);

	//�� �ٲٱ�(��Ƽĳ��Ʈ)
	UFUNCTION(Reliable, NetMulticast)
		void SetisMoving_Multicast(bool value);

	UPROPERTY(replicated)
		bool isObject = false; //�繰�������� �ƴ���

	//Machine Interaction
	void Interaction();

	//Jump
	//UFUNCTION(Reliable, Server)
	//virtual void myJump();

	//���ſ� �̵� ==========
	virtual void UpDown(float f);
	virtual void LeftRight(float f);
	virtual void MyJump();

	//ī�޶�
	virtual void Turn(float f);
	virtual void LookUp(float f);

	//=======
	void JumpAction();

	//ItemBtn
	void Item1();
	void Item2();

private:
	float jumpCntTime = 0.f; //���� �� ������ �ð��� ��� 
	const float jumpEnableTime = 2.0f;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;
private:
	UPROPERTY(replicated)
	float fLeftRight;
	UPROPERTY(replicated)
	float fUpdown;


protected:
	UMyAnimInstance* m_AnimInst; //Ŭ���� ��ü �޾ƿ� ��


public:
	//ĳ������ ����
		//���ø����̼� ����
	UPROPERTY(replicated, ReplicatedUsing = OnRep_State, EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	EPLAYER_STATE m_state;

public:
	//ĳ���� ���� ��ȯ
	UFUNCTION(Reliable, Server)
		virtual void ChangeState(EPLAYER_STATE newState);

	UFUNCTION(Reliable, NetMulticast)
		void ChangeState_Multicast(EPLAYER_STATE newState);

public:
	EPLAYER_STATE GetState() { return m_state; }

protected:
	//������ ���̺�
	UDataTable* m_PlayerTable;

public:
	//ĳ���� ���� �ű��
	virtual void ShareInfoToObject(AMyPlayerObjectPawn* m_PlayerObjectPawn, FTableRowBase data) {}; //�ΰ� -> �繰
	virtual void SetInfoToCharacter(AMyPlayerObjectPawn* m_PlayerObjectPawn,FTableRowBase data) {}; //�繰 -> �ΰ�

	//��Ƽ�÷���==============================================
	UFUNCTION()
		void OnRep_State();

	/** Property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//�ҹ�� ����============================================
	virtual void TurnMove() {};
};
