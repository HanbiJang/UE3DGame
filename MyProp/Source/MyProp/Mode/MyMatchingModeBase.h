// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyMatchingModeBase.generated.h"

/**
 * 
 */
class UMyGameInstance;
UCLASS()
class MYPROP_API AMyMatchingModeBase : public AGameModeBase
{
	GENERATED_BODY()

		AMyMatchingModeBase();

	//��Ʈ�ѷ����� UI ����, ��忡���� �ο����� üũ�ؼ� ���� ���Ӹ����� �̾��ش�

private:
	void PostLogin(APlayerController* NewPlayer) override; //�α����� �����ѵڿ� ȣ��

	int playerNum = 0;

	void Tick(float DeltaTime) override;


		void UpdatePlayerNum();

	UMyGameInstance* GI;

	void BeginPlay();

	FTimerHandle FGameStartTimer;

	void GoGameMap();
	
};
