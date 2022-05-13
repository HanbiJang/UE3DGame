// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyCharacterState.h>
#include "Components/Button.h"
#include <MyProp/GameInstance/MyGameInstance.h>

//��Ī
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyStartGameWidget.generated.h"

class UMyStartHUD;
class USelectCharacter;

/**
 * 
 */
UCLASS()
class MYPROP_API UMyStartGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

	//Ȩ ��ư
	//������ ��ư
	//���θ� ��ư
	//�ƹ��ų� ��ư
	//���ӽ��� ��ư
	UButton* m_HomeBtn;
	UButton* m_RandomBtn;
	UButton* m_KillerBtn;
	UButton* m_SurvivorBtn;
	UButton* m_StartGameBtn;

public:

	//Ȩ ��ư : Ȩȭ������ �ǵ��ư���
	UButton* GetHomeBtn() { return m_HomeBtn; }
	UButton* GetRandomBtn() { return m_RandomBtn; }
	UButton* GetKillerBtn() { return m_KillerBtn; }
	UButton* GetSurvivorBtn() { return m_SurvivorBtn; }
	UButton* GetStartGameBtn() { return m_StartGameBtn; }

public:
	//��ư �� ���
	UFUNCTION()
		void RandomBtnPressed();

	UFUNCTION()
		void KillerBtnPressed();

	UFUNCTION()
		void SurvivorBtnPressed();

	UFUNCTION()
		void StartGameBtnPressed();


public:
	//��Ī �ý��� ===================================
	void CreateServer();
	void FindServer();
	void Init();

	void MySession();
	virtual void OnCreateSessionComplete(FName ServerName, bool Succeded); //���� ������ �������� ȣ���ϴ� �Լ�  
	IOnlineSessionPtr SessionInterface;
};
