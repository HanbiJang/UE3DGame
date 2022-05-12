// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//UI
#include <MyProp/UI/MyMainHUD.h>
#include <MyProp/UI/MyHPBarWidget.h>
#include <MyProp/UI/MySPWidget.h>

#include <MyProp/UI/MyStartGameWidget.h>
#include <MyProp/UI/Killer/MyKillerMainHUD.h>

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyPropGameModeBase.generated.h"

class AKiller;
class ASurvivor;
class AMyPlayerController;

/**
 * 
 */
UCLASS()
class MYPROP_API AMyPropGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	//MyCharacter���� �� ���� ��Ű��
	AMyPropGameModeBase();

	void BeginPlay();
	void Tick(float DeltaTime) override;

	//�÷��̾�� �ִ� ���� ��
	int maxPlayerNum;
	
	//�������� ų���� �ǰ� ������ Ŭ��� �����ڰ� ��
	TArray<AMyPlayerController*> PCSurvivorArr;
	AMyPlayerController* PCKiller;

	//AKiller* pKillerPlayer;
	//ASurvivor* pSurvivorPlayer1;
	//AMyPlayerController* Survivor1Controller;
	//AMyPlayerController* Killer1Controller;

	//�÷��̾� ���� ���� ��ġ (y���� �� ���� ���� �Ÿ� 100�� ���ؼ� ������ ��)
	FVector vKillerSpawnLocation = FVector(240, 170, 200);
	FVector vSurvivor1SpawnLocation = FVector(240, 940, 200);

private:
	/*virtual */void PostLogin(APlayerController* NewPlayer) override; //�α����� �����ѵڿ� ȣ��

	//Ÿ�̸�=============================================================
private:
	float GameLeftTimeSec; //���� ���� �ð��� �ʷ� ȯ���� �ð� : 7��
	UMyGameInstance* GI; //���� �ν��Ͻ�
public:
	float GetGameLeftTimeSec() { return GameLeftTimeSec; }
	void SetGameLeftTimeSec(float value) { GameLeftTimeSec = value; }
	bool IsGameStartEnable; //���� ��ŸƮ�� �������� ����

	UFUNCTION(Reliable, Server)
		void UpdateTimerUI(float DeltaTime);
};
