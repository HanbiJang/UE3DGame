// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerObjectPawn.h"
#include "MyCharacter.h"

// Sets default values
AMyPlayerObjectPawn::AMyPlayerObjectPawn()
	: fRunPower(5),
	fRoPower(0.1f),
	fJumpPower(500)
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
}

//void AMyPlayerObjectPawn::SetSpawnPos(FVector new_originalPos) {
//	originalPos = new_originalPos;
//}

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
	if (m_ObjectMesh->IsSimulatingPhysics()) {
		m_ObjectMesh->AddImpulse(FVector(0, 0, fJumpPower), NAME_None, true);
		UE_LOG(LogTemp, Log, TEXT("Jump!"));
	}
}

void AMyPlayerObjectPawn::Interaction() {

}

void AMyPlayerObjectPawn::PlayerObject() {

	//���� �����ϱ� (�ΰ� ������ ���ư���)

	if (nullptr != pCharacter)
	{
		GetWorld()->GetFirstPlayerController()->Possess(pCharacter);

		FVector originalPos = m_ObjectMesh->GetRelativeLocation(); //�ΰ��� ���ư��� ��� ���� [�޽���] ��ġ ����

		//���� ����
		m_ObjectMesh->SetSimulatePhysics(false);


		//���� [�ΰ���] ������Ʈ �ٸ� ������ ġ��� && ���� ���۰� ���� �����ϱ�
		SetActorLocation(FVector(0, 0, 0));
		SetActorRotation(FRotator(0, 0, 0));
		//�Ž��� ���� �ʱ�ȭ �ϱ�
		m_ObjectMesh->SetAllPhysicsRotation(FRotator(0, 0, 0));
		//[�޽�] �κ� �̵�
		m_ObjectMesh->SetRelativeLocation(FVector(0, 0,
			m_ObjectMesh->GetStaticMesh()->GetBoundingBox().GetSize().Z * GetActorScale().Z));

		//�ΰ����� ��ġ�� ������Ʈ�� �ִ� ���� ��ġ�� �����ϱ�
		pCharacter->SetActorLocation(originalPos);

	}
}