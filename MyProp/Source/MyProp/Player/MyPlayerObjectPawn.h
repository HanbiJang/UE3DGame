// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyInfo.h>
#include <EngineMinimal.h>

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPlayerObjectPawn.generated.h"

//class AMyCharacter; //�ߺ���� ���� �ذ�
class ASurvivor;

UCLASS()
class MYPROP_API AMyPlayerObjectPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPlayerObjectPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//�Ž�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* m_ObjectMesh;

	//ī�޶�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* m_Cam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* m_Arm;

	//�̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRunPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fRoPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fJumpPower;

	//Moving
	void UpDown(float f);
	void LeftRight(float f);

	//Machine Interaction
	void Interaction();

	//����
	void PlayerObject();
	ASurvivor* pCharacter; //�÷��̾� �ΰ��� ����
	void SetPCharacter(ASurvivor* new_pCharacter){ pCharacter = new_pCharacter;  };

	//Jump
	void Jump();

	//Jump ���� ����
	bool isGround;
	//2�� ���� ����
	int JumpCnt = 0;

	//���� ����� �� isGround�� true�� �����
protected:
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:
	//���� �ʱ�ȭ ����, �ʱ�ȭ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	//�� ����
	void ChangeObjectMesh(UStaticMesh* mesh, FVector scale);

	FTimerHandle FPhysicsTimer; //���� �ۿ� ��/�� Ÿ�̸�
	void SetSimulatePhysicsTrue() { m_ObjectMesh->SetSimulatePhysics(true); }

	//���� ���� ���� ��Ÿ���� ���� (���� ���� ��� ������)
	bool bChangeEnable;
	FTimerHandle FChangeEnableTimer; //���� ���� �ð� ��/�� Ÿ�̸�
	void SetbChangeEnableTrue() { bChangeEnable = true; }


private:
	//ĳ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FSurvivorInfo m_Info;
public:
	FSurvivorInfo* GetInfo() { return &m_Info; }
	void SetInfo(FSurvivorInfo new_Info) { m_Info = new_Info; }
};
