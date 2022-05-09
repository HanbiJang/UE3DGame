// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Anim/MyAnimInstance.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "MyPlayerObjectPawn.h"

#include <MyProp/Player/Survivor/Survivor.h>

// Sets default values
AMyCharacter::AMyCharacter() :
	isMoving(false),
	isDashed(false),
	isDashPressed(false),
	isDashEnable(true),
	isJumping(false),
	m_state(EPLAYER_STATE::IDLE),
	isObject(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�Ž� ����
		//���� (����) ��Ų:
		//SkeletalMesh'/Game/PolygonPrototype/Meshes/Characters/SK_Character_Dummy_Male_01.SK_Character_Dummy_Male_01'
		
	//CreateDefaultSubObject ���ϴ� ������Ʈ�� ������	

	//Mesh ���۷��� ��η� ã�ƿ���
	ConstructorHelpers::FObjectFinder<USkeletalMesh> BodyMesh(
		TEXT("SkeletalMesh'/Game/PolygonPrototype/Meshes/Characters/SK_Character_Dummy_Male_01.SK_Character_Dummy_Male_01'"));

	if (BodyMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(BodyMesh.Object);
		
	}

	//ī�޶� ����
	m_Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Arm"));

	m_Arm->SetupAttachment(RootComponent); //������Ʈ�� ī�޶� ���̱�

	//ī�޶� ��ġ ����
	//m_Arm->TargetArmLength = 800.f;
	//m_Arm->TargetOffset = FVector(300, 0, 500);
	//m_Cam->SetWorldRotation(FRotator(-50, 0, 0));
	m_Arm->TargetArmLength = 300.f;
	m_Arm->TargetOffset = FVector(0, 0, 40);
	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //ī�޶�Ͽ� ī�޶� ���̱�

	m_Arm->bUsePawnControlRotation = true;
	m_Cam->bUsePawnControlRotation = false;
	

	//3��Ī ȸ�� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);

	//�ִϸ��̼� ����
	m_AnimInst = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance()); //ĳ������ �Žÿ��� ������ AnimInstance Ŭ���� ��������

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"),this, &AMyCharacter::LeftRight);


	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::MyJump);

	//ItemBtn
	PlayerInputComponent->BindAction(TEXT("Item1"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item1);
	PlayerInputComponent->BindAction(TEXT("Item2"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item2);

	//camera
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);

	//��ȣ�ۿ�
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interaction);

	// ���θ� (���� ��ų �߰�)
	// ������ (Interaction �߰�)

}

void AMyCharacter::Turn(float f) {
	AddControllerYawInput(f);
}

void AMyCharacter::LookUp(float f) {
	AddControllerPitchInput(f);
}

void AMyCharacter::UpDown(float f) {
	fUpdown = f;

	// ���Ʒ��� �̵�
	if (!isObject) {
		if (m_state == EPLAYER_STATE::CATCH || m_state == EPLAYER_STATE::DASH
			|| (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //�����̹���
			|| m_state == EPLAYER_STATE::IDLE || m_state == EPLAYER_STATE::JUMP
			|| m_state == EPLAYER_STATE::MOVE) {
			if (f != 0.f) {
				//��ó� �����϶� �ִϸ��̼� = ���������� Move�� �ȵ�
				TurnMove();
				//if ((!isDashed && !isJumping) 
				//	||m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
				SetisMoving_Server(true);

				//ĳ���� ȸ���� �̵�
				FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
				//GetForwardVector: r��ŭ ���� ������ ���͸� ȸ��
				AddMovementInput(UKismetMathLibrary::GetForwardVector(r), f);
			}
			else if (fUpdown == 0 && fLeftRight == 0 && !isJumping && !isMoving) {
				if (m_state != EPLAYER_STATE::IDLE && m_state != EPLAYER_STATE::HIT) {
					UE_LOG(LogTemp, Log, TEXT("state idle change"));
					SetisMoving_Server(false);
					ChangeState(EPLAYER_STATE::IDLE);
				}
			}
		}
	}

}

void AMyCharacter::LeftRight(float f) {

	fLeftRight = f;
	//������, �������� �̵�
	if (!isObject) {
		if (m_state == EPLAYER_STATE::CATCH || m_state == EPLAYER_STATE::DASH
			|| (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //�����̹���
			|| m_state == EPLAYER_STATE::IDLE || m_state == EPLAYER_STATE::JUMP
			|| m_state == EPLAYER_STATE::MOVE) {
			if (f != 0.f) {
				//��ó� �����϶� �ִϸ��̼� = ���������� Move�� �ȵ�
				TurnMove();
				//if ((!isDashed && !isJumping) || m_state != EPLAYER_STATE::HIT) ChangeState(EPLAYER_STATE::MOVE);
				SetisMoving_Server(true);
				//isMoving = true;

				//ĳ���� ȸ���� �̵�
				FRotator r = FRotator(0, GetControlRotation().Yaw, 0);
				//GetForwardVector: r��ŭ ���� ������ ���͸� ȸ��
				AddMovementInput(UKismetMathLibrary::GetRightVector(r), f);
			}

			else if (fUpdown == 0 && fLeftRight == 0 && !isJumping) {
				if (m_state != EPLAYER_STATE::IDLE && m_state != EPLAYER_STATE::HIT) {
					UE_LOG(LogTemp, Log, TEXT("state idle change"));
					SetisMoving_Server(false);
					ChangeState(EPLAYER_STATE::IDLE);
				}
			}
		}
	}
}

void AMyCharacter::MyJump() {
	if ( (m_state != EPLAYER_STATE::OBJECT && m_state != EPLAYER_STATE::DASH)
		 || (m_state == EPLAYER_STATE::HIT && Cast<ASurvivor>(this)) //�����̹���
		) {
		isJumping = true;
		UE_LOG(LogTemp, Log, TEXT("Jump"))
		ChangeState(EPLAYER_STATE::JUMP);
	}
}

void AMyCharacter::JumpAction() {
	ACharacter::Jump();
}

void AMyCharacter::Dash()
{

}

void AMyCharacter::DashStop()
{

}

void AMyCharacter::Interaction()
{
	if (m_state != EPLAYER_STATE::OBJECT) {
		//���� ��ȯ (��� �ı�, ��� ����)
		ChangeState(EPLAYER_STATE::MACHINE);
	}

}



void AMyCharacter::Item1() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item1 used"));
}

void AMyCharacter::Item2() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item2 used"));
}

void AMyCharacter::ChangeState_Implementation(EPLAYER_STATE newState)
{
	//if (m_state == newState) {
	//	return;
	//}
	////else
	//m_state = newState;
	//UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);
	ChangeState_Multicast(newState);
}

void AMyCharacter::ChangeState_Multicast_Implementation(EPLAYER_STATE newState) {

	if (m_state == newState) {
		return;
	}
	//else
	//�������� Idle�� ���ƿ� �� �������� �ƴ��� �˸�
	if(m_state == EPLAYER_STATE::JUMP && newState == EPLAYER_STATE::IDLE) isJumping = false;
	m_state = newState;
	UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);

}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//ĳ���� ���¸� ���ø�����Ʈ�Ѵ�
	DOREPLIFETIME(AMyCharacter, m_state);

	//ĳ���� �̵� ���� (Idle ���·� �ٲٱ� ����)
	DOREPLIFETIME(AMyCharacter, isMoving);
	DOREPLIFETIME(AMyCharacter, fLeftRight);
	DOREPLIFETIME(AMyCharacter, fUpdown);
	DOREPLIFETIME(AMyCharacter, isJumping);

	DOREPLIFETIME(AMyCharacter, isDashed);
	DOREPLIFETIME(AMyCharacter, isDashPressed);
	DOREPLIFETIME(AMyCharacter, isDashEnable);

	DOREPLIFETIME(AMyCharacter, isObject);
}

//state���� �ٲ�� ȣ���
void AMyCharacter::OnRep_State() {

	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString stateMessage = FString::Printf(TEXT("Survivor State Changed: %i"), m_state);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, stateMessage);
	}

	//Server-specific functionality
	//if (Role == ROLE_Authority)
	if (HasAuthority())
	{
		FString stateMessage = FString::Printf(TEXT("Killer State Changed: %i"), m_state);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, stateMessage);
	}

	//����, Ŭ���̾�Ʈ ���� �κ�

}

//���

void AMyCharacter::SetisDashEnable_Server_Implementation(bool value) {
	SetisDashEnable_Multicast(value);
}

void AMyCharacter::SetisDashEnable_Multicast_Implementation(bool value) {
	isDashEnable = value;
}

void AMyCharacter::SetisDashPressed_Server_Implementation(bool value) {
	SetisDashPressed_Multicast(value);
}

void AMyCharacter::SetisDashPressed_Multicast_Implementation(bool value) {
	isDashPressed = value;
}

//�� �ٲٱ�(����)
void AMyCharacter::SetisDashed_Server_Implementation(bool value) {
	SetisDashed_Multicast(value);
}

//�� �ٲٱ�(��Ƽĳ��Ʈ)
void AMyCharacter::SetisDashed_Multicast_Implementation(bool value) {
	isDashed = value;
}

//�� �ٲٱ�(����)
void AMyCharacter::SetisMoving_Server_Implementation(bool value) {
	SetisMoving_Multicast(value);
}

//�� �ٲٱ�(��Ƽĳ��Ʈ)
void AMyCharacter::SetisMoving_Multicast_Implementation(bool value) {
	isMoving = value;
}