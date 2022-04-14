// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EngineMinimal.h>

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacterObject.generated.h"

class AMyCharacter; //�ߺ���� ���� �ذ�

UCLASS()
class MYPROP_API AMyCharacterObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacterObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


//move

//jump 2ȸ ����


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
	AMyCharacter* pCharacter; //�÷��̾� �ΰ��� ����
	void SetPCharacter(AMyCharacter* new_pCharacter) { pCharacter = new_pCharacter; };

	//Jump
	void Jump();

	//Jump ���� ����
	bool isGround;
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
};
