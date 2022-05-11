// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyMachine.generated.h"


class AKiller;
class ASurvivor;

UCLASS()
class MYPROP_API AMyMachine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() //Box�� �Ҵ�Ǵ� �̺�Ʈ
		void OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit);

	//�Ž�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Mesh; //�繰�� ����ƽ �Ž� (���� ��� ����)
	//������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UCapsuleComponent* m_Cap;

	//����Ʈ ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UPointLightComponent* m_Light;

	UStaticMesh* m_DoneMesh;

	bool IsEnable; //������ ��������
	float Range; //���� �Ÿ�
	float RepairTime; //�� �����ؾ��ϴ� �ð�
	float CurRepairTime; //���� �����ؾ��ϴ� �ð�
	float IsDone; //���� �Ϸ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* RepairBox;
	ASurvivor* sur; //1�� ������


	UFUNCTION(Reliable, Client)
		void MachineUIVisiblity_Client(bool b, ASurvivor* _sur);
};
