// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"

AMyPropGameModeBase::AMyPropGameModeBase() {

	//ĳ���� �������Ʈ Ŭ���� ��������
	//������Ÿ�� ���� �������Ʈ
		//Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor'

	ConstructorHelpers::FClassFinder<APawn>
		MainCharacterClass(TEXT("Blueprint'/Game/Blueprints/BP_Survivor.BP_Survivor_C'"));
	if (MainCharacterClass.Succeeded())
	{
		DefaultPawnClass = MainCharacterClass.Class;
	}

}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();


}
