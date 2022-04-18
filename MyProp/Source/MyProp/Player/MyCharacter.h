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

	//ĳ������ ������Ʈ ��ȯ
public:
	void ChangeToObject(UStaticMesh* mesh, FVector fscale);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		AMyPlayerObjectPawn* m_PlayerObjectPawn; //�繰�� ��

	//���� �ʱ�ȭ ����, �ʱ�ȭ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;


private:
	//ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FTableRowBase m_Info;

protected:
	//������ ���̺�
	UDataTable* m_PlayerTable;

public:
	FTableRowBase* GetInfo() { return &m_Info; }
	void SetInfo(FTableRowBase new_Info) { m_Info = new_Info; }

};
