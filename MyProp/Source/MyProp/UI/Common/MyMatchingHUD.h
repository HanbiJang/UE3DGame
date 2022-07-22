// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Kismet/GameplayStatics.h"

//UI
#include "Components/TextBlock.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyMatchingHUD.generated.h"

/**
 * 
 */
class UMyGameInstance;
UCLASS()
class MYPROP_API UMyMatchingHUD : public UUserWidget
{
	GENERATED_BODY()

public:
		//�÷��̾� �ο��� ��Ÿ���� �ؽ�Ʈ
		UTextBlock* TB_MatchingPlayer;

private:
	/*virtual*/ void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& Geometry, float DT) override;

public:
	UFUNCTION(reliable, Server)
		void UpdatePlayerNum_Server(int num);

	//���� �ν��Ͻ�
	UMyGameInstance* GI;

};
