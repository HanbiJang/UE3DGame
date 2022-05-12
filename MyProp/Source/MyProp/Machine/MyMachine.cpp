// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMachine.h"
#include <MyProp/Player/Killer/Killer.h>
#include <MyProp/Player/Survivor/Survivor.h>
#include <MyProp/Controller/MyPlayerController.h>
#include <MyProp/GameInstance/MyGameInstance.h>
#include <MyProp/Mode/MyPropGameModeBase.h>

// Sets default values
AMyMachine::AMyMachine()
:
IsEnable(true),
Range(100.f), //���� �Ÿ�
RepairTime(10.f), //�� �����ؾ��ϴ� �ð�
CurRepairTime(0.f),
IsDone(false) //���� �Ϸ� ����
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ĸ�� ������Ʈ
	m_Cap = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = m_Cap;

	//�Ž�
	m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_Mesh->SetupAttachment(RootComponent);

	//�ڽ� ���� ����
	RepairBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RepairBox"));
	RepairBox->SetupAttachment(m_Mesh);
	//�ڽ� range ����
	RepairBox->SetBoxExtent(FVector(Range, Range, Range));
	//������ �̺�Ʈ ����
	RepairBox->SetGenerateOverlapEvents(true);

	//����Ʈ
	m_Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("DoneLight"));
	m_Light->SetupAttachment(m_Mesh);
	m_Light->SetIntensity(0.f);
	UE_LOG(LogTemp, Log, TEXT("Intensity : %f"), m_Light->Intensity);

	//�� ������� ���� �Ž�
	ConstructorHelpers::FObjectFinder<UStaticMesh> DoneMeshAsset(
		TEXT("StaticMesh'/Game/Blueprints/MyMesh/SM_PROXY_SM_press_box_Done.SM_PROXY_SM_press_box_Done'"));
	if (DoneMeshAsset.Succeeded())
		m_DoneMesh = DoneMeshAsset.Object;
}

// Called when the game starts or when spawned
void AMyMachine::BeginPlay()
{
	Super::BeginPlay();
	
	//������ �̺�Ʈ���� �����ϱ�
	RepairBox->OnComponentBeginOverlap.AddDynamic(this, &AMyMachine::OnBeginOverlap);
	RepairBox->OnComponentEndOverlap.AddDynamic(this, &AMyMachine::OnEndOverlap);

}

// Called every frame
void AMyMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDone && CurRepairTime >= RepairTime) {//���� �Ϸ�	
		IsDone = true; 
		m_Light->SetIntensity(10000.f);//����Ʈ �ѱ� 15000.
		//�Ž� ����
		if(m_DoneMesh)
			m_Mesh->SetStaticMesh(m_DoneMesh);
		//������ ���� ����
		if(sur){
			sur->ChangeState(EPLAYER_STATE::IDLE);
		}

		//��� ���� ��Ʈ�ѷ����� Ÿ�̸� �ð�, ������ ���� ����
		for (FConstControllerIterator itr = GetWorld()->GetControllerIterator(); itr; ++itr) {
			AMyPlayerController* PC = Cast<AMyPlayerController>(*itr);
			if (PC) {
				PC->SetMachineDone(true); //�Ϸ��� ������ ���� ����
				int machineNum = PC->GetDoneMachineNum();
			}
			//Ÿ�̸� �ð� ����
			//PC->SetGameLeftTimeSec(PC->GetGameLeftTimeSec() + 120.f);
			//AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(GetWorld()->GetMode);
			SetTimerTime_Server(120.f);
		}

		
	}

	if (!IsDone && IsEnable) { //���� �����ϸ�
		//�������̸� �ð� ���
		if (sur && sur->IsRepairEnable && sur->GetState() == EPLAYER_STATE::MACHINE) {
			CurRepairTime = FMath::Clamp(CurRepairTime+DeltaTime, 0.f, RepairTime);
			//UE_LOG(LogTemp, Log, TEXT("Repair Time: %f"), CurRepairTime);
		}
		//������ ���� �ȿ� �ִٸ� UI ������Ʈ
		if (sur && sur->IsRepairEnable && !IsDone) {
			//���� �������� ��� ���� UI ������Ʈ (�Ϸ� �ȵ�������)
			AMyPlayerController* PC = Cast<AMyPlayerController>(sur->GetInstigatorController());
			if (PC) {
				float ratio = CurRepairTime / RepairTime;
				PC->UpdateMachineHUD_Survivor(ratio);
			}
				
		}

	}

}

void AMyMachine::SetTimerTime_Server_Implementation(float f) {
	
	//���� ��忡�� Ÿ�̸� �ð� ����
	AMyPropGameModeBase* GM = Cast<AMyPropGameModeBase>(GetWorld()->GetAuthGameMode());
	GM->SetGameLeftTimeSec(GM->GetGameLeftTimeSec() + f); //f�� ��ŭ �߰�

}

void AMyMachine::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{
	//���� �������� ���� ������ true ��
	sur = Cast<ASurvivor>(_OtherActor);
	if (sur != nullptr && !IsDone) {
		sur->SetIsRepairEnable(true);//��ó������ (��������)
		MachineUIVisiblity_Client(true, sur);
	}

}

void AMyMachine::MachineUIVisiblity_Client_Implementation(bool b, ASurvivor* _sur) {

	//���� �������� ��� ���� UI����
	if (_sur != nullptr) {
		AMyPlayerController* PC = Cast<AMyPlayerController>(_sur->GetController());
		if (PC != nullptr) {
			if (PC->GetMainHUD() && PC->GetMainHUD()->GetMachineHUD()) {
				if (b) {
					PC->GetMainHUD()->GetMachineHUD()->SetVisibility(ESlateVisibility::Visible);
				}

				else {
					PC->GetMainHUD()->GetMachineHUD()->SetVisibility(ESlateVisibility::Hidden);
				}
			}
		}
	}

}

void AMyMachine::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{
	//���� �������� ���� ������ false ��
	if (sur) {
		sur->SetIsRepairEnable(false);
		//���� �������� ��� ���� UI���ֱ�
		MachineUIVisiblity_Client(false, sur);

		sur = nullptr; //���� �ٲ������ƶ�
	}
	
}

void AMyMachine::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, FVector _NormalImpulse, const FHitResult& Hit)
{

}



