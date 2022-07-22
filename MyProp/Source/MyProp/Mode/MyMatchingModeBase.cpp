// Fill out your copyright notice in the Description page of Project Settings.


#include <MyProp/Mode/MyMatchingModeBase.h>

#include <MyProp/GameInstance/MyGameInstance.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Controller/MyPlayerController.h>


AMyMatchingModeBase::AMyMatchingModeBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMyMatchingModeBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AMyMatchingModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�ο� �� �ݿ��ϱ�
	UpdatePlayerNum();

}

void AMyMatchingModeBase::UpdatePlayerNum()
{
	//�÷��̾� �ο� �� ����		
	playerNum = GetWorld()->GetNumPlayerControllers();

	if (GI) {
		//��Ʈ�ѷ��� �� �ݿ� 
		for (int i = 0; i < GI->GetMatchingPCArr().Num(); i++)
		{
			if (IsValid((GI->GetMatchingPCArr())[i]))
			{
				(GI->GetMatchingPCArr())[i]->SetPlayerCnt(playerNum);
				(GI->GetMatchingPCArr())[i]->SetPlayerCnt_Client(playerNum);
				(GI->GetMatchingPCArr())[i]->SetID(GetWorld()->GetNumPlayerControllers());
				(GI->GetMatchingPCArr())[i]->SetID_Client(GetWorld()->GetNumPlayerControllers());
			}
		}

		
	}
	else 
	{
		GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}

}

void AMyMatchingModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (GI) {

		//��Ʈ�ѷ� ����
		AMyPlayerController* m_PC = Cast<AMyPlayerController>(NewPlayer);

		if (m_PC != nullptr)
		{

			//for (int i = 0;  i < GI->GetMatchingPCArr().Num(); i++) 
			//{
			//	if (GI->GetMatchingPCArr()[i] == nullptr)
			//	{
			//		GI->GetMatchingPCArr()[i] = (m_PC);
			//		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Blue, FString::Printf(TEXT("when i : %d"), i));
			//		break;
			//	}
			//}

			//GI->GetMatchingPCArr().Add(m_PC);
			GI->AddMatchingPCArr(m_PC);
			UpdatePlayerNum();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Controller Cast Fail")));
			return;
		}


		//GEngine->AddOnScreenDebugMessage( 0, 2.f, FColor::Blue, FString::Printf( TEXT("Player Number : Gi - %d playerNum : %d"), GI->iPlayerCnt , playerNum) );
	}
	else {
		//Ÿ��Ʋ�� ���ư���		
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMap"));
		return;
	}

	if(GetWorld()->GetNumPlayerControllers() == GI->maxPlayer)
		GetWorld()->GetTimerManager().SetTimer(FGameStartTimer, this, &AMyMatchingModeBase::GoGameMap, 5.0f, false);
	//5�� ���� �ð�

}

void AMyMatchingModeBase::GoGameMap() 
{
	FString UrlString = TEXT("InGameMap");
	GetWorld()->ServerTravel(UrlString);
}
