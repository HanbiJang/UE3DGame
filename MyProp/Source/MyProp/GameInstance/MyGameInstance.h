// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/Player/Common/MyInfo.h>
#include <Engine/DataTable.h>
#include <MyProp/Player/Common/MyCharacterState.h>
//#include <MyProp/Player/Killer/Killer.h>
//#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/UI/MyMainHUD.h>

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MYPROP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UDataTable* m_SurvivorTable;
	UDataTable* m_KillerTable;

	//���θ��� ������ �������Ʈ ����
	TSubclassOf<APawn> m_Killer;
	TSubclassOf<APawn> m_Survivor;

	//UI
	TSubclassOf<UUserWidget> m_KillerWidgetClass;
	TSubclassOf<UUserWidget> m_SurvivorWidgetClass;

public:
	TSubclassOf<UUserWidget> m_LoadingWidget;

public:
	//�÷��̾� ��
	int iPlayerCnt = 0;
	int maxPlayer = 5;


public:
	const TSubclassOf<APawn> GetKiller() {
		return m_Killer;
	};
	const TSubclassOf<APawn> GetSurvivor() {
		return m_Survivor;
	};
	const FSurvivorInfo* GetSurvivorInfo(const FString& _RowName);
	const FKillerInfo* GetKillerInfo(const FString& _RowName);

	//UI
	const TSubclassOf<UUserWidget> GetSurvivorWidgetClass() { return m_SurvivorWidgetClass; };
	const TSubclassOf<UUserWidget> GetKillerWidgetClass() { return m_KillerWidgetClass; };

	//ĳ���� ����
private:
	EPLAYER_TYPE m_SelectType;
public:
	EPLAYER_TYPE GetSelectType() { return m_SelectType; };
	void SetSelectType(EPLAYER_TYPE value) { m_SelectType = value; }

public:
	UMyGameInstance();
	~UMyGameInstance();

	//�÷��̾� �̸�
	FString MyPlayerName;

	//�̹���
	UTexture2D* Machine_DoneImg;
	UTexture2D* GetDoneImg() {
		return Machine_DoneImg;
	}
	
	//�ٸ� �÷��̾� �̹���
	TArray<UTexture2D*> AllPlayerImg;
	TArray<UTexture2D*> GetAllPlayerImg() {
		return AllPlayerImg;
	};
	UTexture2D* GetPlayerImg(int idx) {
		return AllPlayerImg[idx];
	};

};
