// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"//��������
#include "Kismet/GameplayStatics.h"
#include "Components/EditableTextBox.h"

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

	//�÷��̾� �̸� ����
	//ET_MyPlayerName
	UEditableTextBox* ET_MyPlayerName;

	//������ ��ư : ���� �����ϱ�
	UButton* m_ExitBtn;

public:
	UButton* GetPlayBtn() { return m_PlayBtn;}
	UButton* GetExitBtn() { return m_ExitBtn; }
	UMyStartGameWidget* GetStartGameWidget() { return m_StartGameWidget; }
	UEditableTextBox* GetET_MyPlayerName() { return ET_MyPlayerName; }
	
	//�÷��� ��ư : ���� ���� �г� ����
	UFUNCTION()
		void ShowStartGameUI();

	//������ ��ư : ���� �����ϱ�
	UFUNCTION()
		void ExitGame();

	//Ȩȭ�� ���ư��� ��ư
	UFUNCTION()
		void ShowHomeUI();

	//���� ���� (���ε��� �Լ�)
	UFUNCTION()
		void OnMyPlayerNameTextChange(const FText& Text);

};
