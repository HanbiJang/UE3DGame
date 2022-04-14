// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Anim/MyAnimInstance.h"

#include "Engine/BlueprintGeneratedClass.h"
#include "MyPlayerObjectPawn.h"

// Sets default values
AMyCharacter::AMyCharacter() :
	isDashed(false),
	m_state(EPLAYER_STATE::IDLE),
	FVChange(0, 0, 0),
	FRChange(0, 0, 0)
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
	m_Arm->TargetArmLength = 800.f;
	m_Arm->TargetOffset = FVector(300, 0, 500);
	m_Cam->SetWorldRotation(FRotator(-50, 0, 0));

	m_Arm->bUsePawnControlRotation = true;

	m_Cam->SetupAttachment(m_Arm, USpringArmComponent::SocketName); //ī�޶�Ͽ� ī�޶� ���̱�

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

	//���ſ� �繰 ������Ʈ
	//��.�� ������Ʈ ����
	UBlueprintGeneratedClass* LoadBP = LoadObject<UBlueprintGeneratedClass>(GetWorld(),
		TEXT("Blueprint'/Game/Blueprints/Objects/BP_MyPlayerObjectPawn.BP_MyPlayerObjectPawn_C'"));

	if (LoadBP)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		m_PlayerObjectPawn = nullptr;

		m_PlayerObjectPawn = GetWorld()->SpawnActor<AMyPlayerObjectPawn>(LoadBP,
			FVector(0, 0, 0),//��ü�� �뷫�� ũ�� ���ϱ�
			FRotator(0, 0, 0),
			SpawnInfo);
		m_PlayerObjectPawn->SetPCharacter(this); //�ΰ��� �������
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//����
	if (fLeftRight == 0.f && fUpdown == 0.f && m_state == EPLAYER_STATE::MOVE) {
		ChangeState(EPLAYER_STATE::IDLE);
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Moving
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Repeat, this, &AMyCharacter::Dash); //������ ���� ��� ����
	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Released, this, &AMyCharacter::DashStop); //��� ����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"),this, &AMyCharacter::LeftRight);


	//Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	//ItemBtn
	PlayerInputComponent->BindAction(TEXT("Item1"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item1);
	PlayerInputComponent->BindAction(TEXT("Item2"), EInputEvent::IE_Pressed, this, &AMyCharacter::Item2);


	//��ȣ�ۿ�
	PlayerInputComponent->BindAction(TEXT("Interaction"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interaction);

	// ���θ� (���� ��ų �߰�)
	// ������ (Interaction �߰�)

}

void AMyCharacter::UpDown(float f) {
	// ���Ʒ��� �̵�
	if ( f != 0.f && m_state != EPLAYER_STATE::JUMP && m_state != EPLAYER_STATE::DASH ) {
		ChangeState(EPLAYER_STATE::MOVE);
	}

	fLeftRight = f;
		
	//ĳ���� ȸ���� �̵�
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, f);
}

void AMyCharacter::LeftRight(float f) {
	//������, �������� �̵�
	if (f != 0.f && m_state != EPLAYER_STATE::JUMP && m_state != EPLAYER_STATE::DASH && !isDashed) {
		ChangeState(EPLAYER_STATE::MOVE);
	}

	fUpdown = f;

	//ĳ���� ȸ���� �̵�
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, f);
}

void AMyCharacter::Dash()
{
	isDashed = true;

	if (m_state != EPLAYER_STATE::OBJECT) {
		//�ӵ� ���� (���)
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;

		//���� ��ȯ
		ChangeState(EPLAYER_STATE::DASH);
	}
}

void AMyCharacter::DashStop()
{
	isDashed = false;

	if (m_state != EPLAYER_STATE::OBJECT) {
		//�ӵ� ���󺹱�
		GetCharacterMovement()->MaxWalkSpeed = 600.f;

		//���� ��ȯ
		ChangeState(EPLAYER_STATE::MOVE);
	}
}

void AMyCharacter::Interaction()
{
	if (m_state != EPLAYER_STATE::OBJECT) {
		//���� ��ȯ (��� �ı�, ��� ����)
		ChangeState(EPLAYER_STATE::MACHINE);
	}

}

void AMyCharacter::Jump() {
	if (m_state != EPLAYER_STATE::DASH) {
		UE_LOG(LogTemp, Log, TEXT("Jump"))
		ChangeState(EPLAYER_STATE::JUMP);
	}

	ACharacter::Jump();

}

void AMyCharacter::JumpAction() {
	ACharacter::Jump();
}

void AMyCharacter::Item1() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item1 used"));
}

void AMyCharacter::Item2() {
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("item2 used"));
}

void AMyCharacter::ChangeState(EPLAYER_STATE newState)
{
	if (m_state == newState) {
		return;
	}

	//else

	m_state = newState;
	UE_LOG(LogTemp, Log, TEXT("state: %i"), m_state);
}

void AMyCharacter::ChangeToObject(UStaticMesh* mesh, FVector fscale)
{
	//�÷��̾� ���� ����
	ChangeState(EPLAYER_STATE::OBJECT);

	FVector originalPos = GetActorLocation(); //���� ��� ���� ��ġ ����

	//���� [�ΰ���] ������Ʈ �ٸ� ������ ġ��� && ���� ���۰� ���� �����ϱ�
	SetActorLocation(FVChange);
	SetActorRotation(FRChange);

	//���� �ѱ�
	m_PlayerObjectPawn->m_ObjectMesh->SetSimulatePhysics(true);

	//��ü ��ġ�� �ΰ����� �ִ� ������ ����
	m_PlayerObjectPawn->m_ObjectMesh->SetAllPhysicsPosition(originalPos + FVector(0, 0, mesh->GetBoundingBox().GetSize().Z * fscale.Z));

	//Ŭ���� ������Ʈ �޽��� ũ��� ���� ����
	m_PlayerObjectPawn->m_ObjectMesh->SetRelativeScale3D(fscale);
	//�Ž� ����
	m_PlayerObjectPawn->m_ObjectMesh->SetStaticMesh(mesh);

	//���� �ű��...
	GetWorld()->GetFirstPlayerController()->Possess(m_PlayerObjectPawn);

	//���� ���� ���� ����
	//1.5�� �ڿ� ���� ����������
	GetWorld()->GetTimerManager().SetTimer(m_PlayerObjectPawn->FChangeEnableTimer, m_PlayerObjectPawn, &AMyPlayerObjectPawn::SetbChangeEnableTrue, 1.5f, false);
}


