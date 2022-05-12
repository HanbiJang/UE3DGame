// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPropGameModeBase.h"
#include <MyProp/GameInstance/MyGameInstance.h>

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/Controller/MyPlayerController.h>

AMyPropGameModeBase::AMyPropGameModeBase():
	GameLeftTimeSec(60.f * 7),
	maxPlayerNum( 3/*5*/ ),
	IsGameStartEnable(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = nullptr;
}

void AMyPropGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void AMyPropGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UpdateTimerUI(DeltaTime);
}

void AMyPropGameModeBase::UpdateTimerUI_Implementation(float DeltaTime) {
	//���� ����
	if (GI->iPlayerCnt == maxPlayerNum && !IsGameStartEnable) {
		IsGameStartEnable = true;
		UE_LOG(LogTemp, Log, TEXT("Start Game!!!!"));
	}

	//���� Ÿ�̸� ==============================================================
	//Ÿ�̸� UI ������Ʈ ========================================================
	if (IsGameStartEnable) {

		//�ð� ������Ʈ
		if (GameLeftTimeSec > 0) {
			GameLeftTimeSec = FMath::Clamp(GameLeftTimeSec - DeltaTime, 0.f, 60 * 99.f);
			if (GameLeftTimeSec <= 0) {
				GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, TEXT("Game Over"));
			}
		}
		//�ð� ��,�ʷ� ���
		FString timeStr;
		int min = GameLeftTimeSec / 60.f;
		int sec = GameLeftTimeSec - min * 60;
		//2�ڸ����� �ð� ǥ���ϱ�
		if (min <= 9) {
			if (sec <= 9) timeStr = FString::Printf(TEXT("0%i:0%i"), min, sec);
			else timeStr = FString::Printf(TEXT("0%i:%i"), min, sec);
		}
		else {
			if (sec <= 9) timeStr = FString::Printf(TEXT("%i:0%i"), min, sec);
			else timeStr = FString::Printf(TEXT("%i:%i"), min, sec);
		}

		//������ Ÿ�̸� UI ������Ʈ
		//UI�� �ȱ׷����� �� ���...
		if(PCKiller->GetKillerMainHUD() && PCKiller->GetKillerMainHUD()->GetTimerHUD())
			PCKiller->GetKillerMainHUD()->GetTimerHUD()->SetTimeText(timeStr);
		//��� Ŭ�� �÷��̾� UI ������Ʈ
		for (int i = 0; i < PCSurvivorArr.Num(); i++) {
			//UI�� Ŭ�� �����̶� UMG�� �׳� �����ͼ� ������ ����.Ŭ�� ���뿡���� ����Ǵ� �Լ��� ����
			PCSurvivorArr[i]->UpdateTimerUI_Client(timeStr);
		}
	}
}

void AMyPropGameModeBase::PostLogin(APlayerController* NewPlayer) {
	Super::PostLogin(NewPlayer);
	
	if(GI == nullptr){
		GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}
	if (GI) {
		//�÷��̾� �ο� �� ����
		GI->iPlayerCnt++;
		UE_LOG(LogTemp, Log, TEXT("player Num : %i"), GI->iPlayerCnt);

		//ĳ���� ���� ����
		FActorSpawnParameters spawnInfo;
		spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//�÷��̾ �� ���̸�, ������ ���θ�, Ŭ��� �����ڷ� ����
		//������ ĳ���Ϳ� ���� (����)
		if (GI->iPlayerCnt == 1) {
			//ĳ���� ���� 
			AKiller* pkiller = GetWorld()->SpawnActor<AKiller>(GI->GetKiller(),
				vKillerSpawnLocation, FRotator(0, 0, 0), spawnInfo);

			PCKiller = Cast<AMyPlayerController>(NewPlayer);
			if (PCKiller && pkiller) {
				PCKiller->Possess(pkiller); //ĳ���� possess
				PCKiller->DrawHUD_Server(); //HUD ���� 
			}

		}

		//Ŭ���̾�Ʈ - ������ ĳ���� ����
		else if (GI->iPlayerCnt >= 2) {
			//ĳ���� ����
			//ĳ���� ���� ���
			FVector fvspawnlo = FVector(
				vSurvivor1SpawnLocation.X,
				vSurvivor1SpawnLocation.Y + (GI->iPlayerCnt - 2) * 100,
				vSurvivor1SpawnLocation.Z
			);
			ASurvivor* psur = GetWorld()->SpawnActor<ASurvivor>(GI->GetSurvivor(),
				fvspawnlo, FRotator(0, 0, 0), spawnInfo);

			AMyPlayerController* pcsur = Cast<AMyPlayerController>(NewPlayer);
			if (pcsur && psur) {
				PCSurvivorArr.Add(pcsur);
				pcsur->Possess(psur); //ĳ���� possess
				pcsur->DrawHUD_Client(); //HUD ���� 
			}
		}
	}
	else {
		//Ÿ��Ʋ�� ���ư���
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMap"));
	}

}