// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/Player/Survivor/Survivor.h>

#include <MyProp/UI/Killer/MyKillerMainHUD.h>
#include <MyProp/Player/Killer/Killer.h>

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class AMyMachine;
class UMyTimerWidget;
class AMyPropGameModeBase;
/**
 * 
 */
UCLASS()
class MYPROP_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

		AMyPlayerController(); //������
public:
	UFUNCTION(Client, reliable)
		void DrawHUD_Client();

	UFUNCTION(Server, reliable)
		void DrawHUD_Server();

	//HUD
	TSubclassOf<UUserWidget> m_SurvivorMainHUDClass;
	UMyMainHUD* m_SurvivorMainHUD;
	UMyMainHUD* GetMainHUD(){ return m_SurvivorMainHUD; };

	//Killer (Server) HUD
	TSubclassOf<UUserWidget> m_KillerMainHUDClass;
	UMyKillerMainHUD* m_KillerMainHUD;
	UMyKillerMainHUD* GetKillerMainHUD() { return m_KillerMainHUD; };

public:
	/*virtual*/ void PlayerTick(float DeltaTime) override;
	void BeginPlay() override;

public:
	//���θ� UI ������Ʈ
	UFUNCTION(Reliable, Server) //�����ؾ���
		void UpdatePlayHUD_Killer(float _CurQTimeRatio, float _CurETimeRatio, float _CurRCTimeRatio
			, float _CurQTime, float _CurETime, float _CurRCTime);

	UFUNCTION(Reliable, Client)
		void UpdatePlayHUD_Survivor(float _CurHPRatio, float _CurSPRatio, float _CurHP, float _MaxHP);

	UFUNCTION(Reliable, Client)
		void UpdateMachineHUD_Survivor(float _CurMachineRatio);

private:
	//������ ���� ��Ʈ�� & UI
	int DoneMachineNum;
	bool IsAllMachineRepaired;

public:
	int GetDoneMachineNum() { return DoneMachineNum; }
	bool GetIsAllMachineRepaired() { return IsAllMachineRepaired; }
	void SetMachineDone(bool b) {
		DoneMachineNum++;
	}

	//���� Ÿ�̸�======================================================
	
	//FTimerHandle FGameTimer; //���� Ÿ�̸�
//private:
	//float GameLeftTimeSec; //���� ���� �ð��� �ʷ� ȯ���� �ð� : 7��
	//void CheckTime(); //���� Ÿ�̸� �ð� üũ
public:
	//float GetGameLeftTimeSec() { return GameLeftTimeSec; }
	//void SetGameLeftTimeSec(float value) { GameLeftTimeSec = value; }
	
	UFUNCTION(Reliable, Client)
		void UpdateTimerUI_Client(const FString& timestr);

};
