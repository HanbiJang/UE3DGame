// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine/DataTable.h>
#include "CoreMinimal.h"
#include "MyInfo.generated.h" //staticStruct ���� �ذ�

/**
 * 
 */
class MYPROP_API MyInfo
{
public:
	MyInfo();
	~MyInfo();
};


//������
USTRUCT(Atomic, BlueprintType)
struct FSurvivorInfo : public FTableRowBase // ������ ���̺�� �����Ǵ� ����ü
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FString strClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxHP; //[������] �ִ� ü��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurHP; //[������] ���� ü��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iMachine; //[������] ������ ���� �� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxSP; //[������] �ִ� ���׹̳�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurSP; //[������] ���׹̳�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iStun; //[������] ���θ� ���� �� 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iCoin; //[����] ���� �� 


};

//���θ�
USTRUCT(Atomic, BlueprintType)
struct FKillerInfo : public FTableRowBase // ������ ���̺�� �����Ǵ� ����ü
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		FString strClassName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fMaxSP; //[����] �ִ� ���׹̳�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fCurSP; //[����] ���׹̳�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iCoin; //[����] ���� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		int iKill; //[���θ�] ų ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		float fAllHit; //[���θ�] �� ���� ����

};

//ų�� ���� ����Ʈ
UENUM(BlueprintType)
enum class EKillerEffect : uint8
{
	ATTACK,
	Q,
	E,
	RIGHTCLICK,
	PARTICLEHIT,

};