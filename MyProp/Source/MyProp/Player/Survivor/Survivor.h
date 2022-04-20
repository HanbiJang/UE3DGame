// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "../../MyPropGameModeBase.h"
#include "../Killer/Killer.h"

#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Survivor.generated.h"

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

	//�̵� (���)
	void Dash() override;
	void DashStop() override;

	void Interaction();

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		AMyPlayerObjectPawn* m_PlayerObjectPawn; //�繰�� ��

	//���� �ʱ�ȭ ����, �ʱ�ȭ ��ġ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FVector FVChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FRotator FRChange;

	void ChangeToObject(UStaticMesh* mesh, FVector fscale);

	//����
	//�߰� ����ڵ� ���� ����
	USoundWave* SW_HeartBeat;
	//�߰� BGM ���� ����
	USoundWave* SW_Chase;
	//���� ������Ʈ
	UAudioComponent* AC_HeartBeat;
	UAudioComponent* AC_Chase;
};
