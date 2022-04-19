// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObjectPawn.h"
//#include "MyCharacter.h"
#include "Survivor/Survivor.h"

// Sets default values
AMyPlayerObjectPawn::AMyPlayerObjectPawn()
	: fRunPower(5),
	fRoPower(0.1f),
	fJumpPower(500),
	FVChange(0,0,0),
	FRChange(0,0,0),
	bChangeEnable(false)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ĸ�� ������Ʈ	
	RootComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	//���� �� �޽�
	m_ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ObjectMesh"));
	m_ObjectMesh->SetupAttachment(RootComponent);

	//ī�޶� ����
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(m_ObjectMesh); //�Žÿ� ī�޶� ���̱�

	m_Arm->bUsePawnControlRotation = true;

	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //ī�޶�Ͽ� ī�޶� ���̱�

	//ī�޶� ��ġ ����
	m_Arm->TargetArmLength = 800.f;
	m_Arm->TargetOffset = FVector(300, 0, 500);
	m_Cam->SetWorldRotation(FRotator(-50, 0, 0));

	m_Cam->bUsePawnControlRotation = false;

	//3��Ī ȸ�� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void AMyPlayerObjectPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPlayerObjectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	////������Ʈ ��ġ�� Physics Location���� ������Ʈ
	//RootComponent->SetWorldLocation(m_ObjectMesh->GetComponentLocation());
}

// Called to bind functionality to input
void AMyPlayerObjectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyPlayerObjectPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyPlayerObjectPawn::LeftRight);

	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::Jump);

	//��ȣ�ۿ�
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::Interaction);

	//�ΰ��� ����
	PlayerInputComponent->BindAction(TEXT("PlayerObject"), EInputEvent::IE_Pressed, this, &AMyPlayerObjectPawn::PlayerObject);
}

void AMyPlayerObjectPawn::UpDown(float f)
{
	if (f != 0 && m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(fRunPower* f, 0, 0), NAME_None, true);
		m_ObjectMesh->AddAngularImpulseInDegrees(FVector(0, fRoPower *f, 0.f), NAME_None, true);
	}

}

void AMyPlayerObjectPawn::LeftRight(float f)
{
	if (f != 0 && m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(0, fRunPower * f, 0.f), NAME_None, true);
		m_ObjectMesh->AddAngularImpulseInDegrees(FVector(fRoPower *f, 0, 0.f), NAME_None, true);
	}
}

void AMyPlayerObjectPawn::Jump()
{
	if (JumpCnt < 2 && m_ObjectMesh->IsSimulatingPhysics()) {
		JumpCnt++;
		isGround = false;
		m_ObjectMesh->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
		UE_LOG(LogTemp, Log, TEXT("Jump!"));
	}
}

void AMyPlayerObjectPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//���� ground�� ġ�� �ʱ�
	//+DefaultChannelResponses=(Channel=ECC_GameTraceChannel2,DefaultResponse=ECR_Block,bTraceType=False,bStaticObject=False,Name="WallObject")
	if (OtherComp->GetCollisionObjectType() != ECC_GameTraceChannel2) {
		if (!isGround) { 
			isGround = true; 
			JumpCnt = 0;
		}
	}
	

}

//�纯�� (���� ����)
void AMyPlayerObjectPawn::ChangeObjectMesh(UStaticMesh* mesh, FVector scale)
{
	if (bChangeEnable) {
		bChangeEnable = false; //���ٺ��� ����

		//���� ����
		m_ObjectMesh->SetSimulatePhysics(false);

		//�Ž��� ���� �ʱ�ȭ �ϱ�
		m_ObjectMesh->SetAllPhysicsRotation(FRChange);
		//[�Ž�] �κ� �̵�
		m_ObjectMesh->SetRelativeLocation(m_ObjectMesh->GetRelativeLocation() + FVector(0, 0,
			m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//�Ž� ����
		m_ObjectMesh->SetStaticMesh(mesh);
		//ũ�� ����
		m_ObjectMesh->SetRelativeScale3D(scale);

		//0.1�� �ڿ� ���� �ѱ�
		GetWorld()->GetTimerManager().SetTimer(FPhysicsTimer, this, &AMyPlayerObjectPawn::SetSimulatePhysicsTrue, 0.1f, false);

		//1.5�� �ڿ� ���� ����������
		GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("bChangeEnable false!"));
	}
}

void AMyPlayerObjectPawn::Interaction() {

}

/// <summary>
/// ���� �����ϱ� (�ΰ� ������ ���ư���)
/// </summary>
void AMyPlayerObjectPawn::PlayerObject() {

	if (bChangeEnable) {

		if (nullptr != pCharacter) //�ܺο��� ���� �������
		{
			bChangeEnable = false; //���ٺ��� ����
			GetWorld()->GetFirstPlayerController()->Possess(pCharacter);

			FVector originalPos = m_ObjectMesh->GetRelativeLocation(); //�ΰ��� ���ư��� ��� ���� [�޽���] ��ġ ����

			//���� ����
			m_ObjectMesh->SetSimulatePhysics(false);


			//���� [�ΰ���] ������Ʈ �ٸ� ������ ġ��� && ���� ���۰� ���� �����ϱ�
			SetActorLocation(FVChange);
			SetActorRotation(FRChange);
			//�Ž��� ���� �ʱ�ȭ �ϱ�
			m_ObjectMesh->SetAllPhysicsRotation(FRChange);
			//[�޽�] �κ� �̵�
			m_ObjectMesh->SetRelativeLocation(FVChange + FVector(0, 0,
				m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

			//�ΰ����� ��ġ�� ������Ʈ�� �ִ� ���� ��ġ�� �����ϱ�
			pCharacter->SetActorLocation(originalPos);

			//1.5�� �ڿ� ���� ����������
			GetWorld()->GetTimerManager().SetTimer(FChangeEnableTimer, this, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);

			//ĳ���� ���� �ű��
			pCharacter->SetInfo(m_Info);
		}
	}
}