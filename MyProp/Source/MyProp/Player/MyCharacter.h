// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyInfo.h" //ĳ���� ���� 
#include <EngineMinimal.h>
#include "MyCharacterState.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class UMyAnimInstance; //�ߺ� ��� ���� ���� �ذ�
class AMyPlayerObjectPawn; //�ߺ� ��� ���� ���� �ذ�

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


	//�Ž� ����
	//�ΰ� ���� ����

	//Moving
	void UpDown(float f);
	void LeftRight(float f);
	void Dash();

	//Machine Interaction
	void Interaction();

	//Jump
	void Jump();
	void JumpAction();
	bool isDashed;
	void DashStop();

	//ItemBtn
	void Item1();
	void Item2();

private:
	float jumpCntTime = 0.f; //���� �� ������ �ð��� ��� 
	const float jumpEnableTime = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	float fLeftRight;
	float fUpdown;


protected:
	UMyAnimInstance* m_AnimInst; //Ŭ���� ��ü �޾ƿ� ��


public:
	//ĳ������ ����
	EPLAYER_STATE m_state;

public:
	//ĳ���� ���� ��ȯ
	virtual void ChangeState(EPLAYER_STATE newState);

public:
	EPLAYER_STATE GetState() { return m_state; }

protected:
	//������ ���̺�
	UDataTable* m_PlayerTable;

public:
	//ĳ���� ���� �ű��
	virtual void ShareInfoToObject(AMyPlayerObjectPawn* m_PlayerObjectPawn, FTableRowBase data) {}; //�ΰ� -> �繰
	virtual void SetInfoToCharacter(AMyPlayerObjectPawn* m_PlayerObjectPawn,FTableRowBase data) {}; //�繰 -> �ΰ�
};
