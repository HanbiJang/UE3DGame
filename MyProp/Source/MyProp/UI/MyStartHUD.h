// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"//��������
#include "Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartHUD.generated.h"

class UMyStartGameWidget;

/**
 * 
 */
UCLASS()
class MYPROP_API UMyStartHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//�÷��� ��ư : ���� ���� �г� ����
	UButton* m_PlayBtn;
	UMyStartGameWidget* m_StartGameWidget;

	//������ ��ư : ���� �����ϱ�
	UButton* m_ExitBtn;

public:
	UButton* GetPlayBtn() { return m_PlayBtn;}
	UButton* GetExitBtn() { return m_ExitBtn; }
	UMyStartGameWidget* GetStartGameWidget() { return m_StartGameWidget; }
	
	//�÷��� ��ư : ���� ���� �г� ����
	UFUNCTION()
		void ShowStartGameUI();

	//������ ��ư : ���� �����ϱ�
	UFUNCTION()
		void ExitGame();

	//Ȩȭ�� ���ư��� ��ư
	UFUNCTION()
		void ShowHomeUI();

};
