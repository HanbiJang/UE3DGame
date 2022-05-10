// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPLAYER_STATE : uint8
{
	IDLE,
	MOVE,
	JUMP,
	DASH,
	HIT, //�����ڰ� ��Ƽ� ����, ���θ����� �¾��� ��
	
	//������ ����
	MACHINE, //������ ����
	DEAD, //���� ����
	OBJECT, //������Ʈ ���� 
	DRAGGED, //���θ����� �������� ����
	LOCKED, //�ָ����ڿ� ���Ӵ��ϱ�

	//ų�� ����
	ATTACK,
	RANGEATTACK, //���� ���� Q��ư
	CATCH, //�����ڸ� ������
	LOCK, //�ָ����ڿ� ����
	//+)
	RCATTACK,
	EATTACK,
	EATTACK_,
};

UENUM(BlueprintType)
enum class EPLAYER_TYPE : uint8
{
	NONE,
	KILLER,
	SURVIVOR,
	RANDOM,
};

class MYPROP_API MyCharacterState
{


public:
	MyCharacterState();
	~MyCharacterState();

};
