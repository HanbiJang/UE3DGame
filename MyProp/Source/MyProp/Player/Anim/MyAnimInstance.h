// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EngineMinimal.h>
#include <MyProp/Player/Common/MyCharacterState.h>

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

class AMyCharacter; //�ߺ���� ���� �ذ�

/**
 * 
 */
UCLASS()
class MYPROP_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	/*virtual */void NativeInitializeAnimation() override;
	/*virtual */void NativeBeginPlay() override;
	/*virtual */void NativeUpdateAnimation(float _fDT) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EPLAYER_STATE m_eState; //ĳ���� ����
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		bool m_bGround; //���鿡 �ִ���

	//�ִϸ��̼� ���� �Լ�

	UFUNCTION()
		void AnimNotify_Idle();

	UFUNCTION()
		void AnimNotify_JumpAction();

	UFUNCTION()
		void AnimNotify_AttackAction();

	UFUNCTION()
		void AnimNotify_RangeAttackAction();

	UFUNCTION()
		void AnimNotify_RCAttackAction();

	UFUNCTION()
		void AnimNotify_DoNotThing() {};

};
