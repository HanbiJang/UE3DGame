// Fill out your copyright notice in the Description page of Project Settings.


#include "KillerRCProjectile.h"
#include "Components/StaticMeshComponent.h"
#include <MyProp/Player/MyEffectManager.h>
#include <MyProp/Player/Survivor/Survivor.h> 


AKillerRCProjectile::AKillerRCProjectile()
	: m_fAccTime(0.f)
	, m_Sound(nullptr)
	, m_Attenuation(nullptr) 
{
	isHit = false;

	//�Ž� ����
	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_MeshComponent->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(
		TEXT("StaticMesh'/Game/PolygonPrototype/Meshes/Props/SM_Prop_Knife_01.SM_Prop_Knife_01'"));
	if (meshAsset.Succeeded()) {	
		m_Mesh = meshAsset.Object;
		m_MeshComponent->SetStaticMesh(m_Mesh);
	}

	//��ƼŬ ���� ����
	ConstructorHelpers::FObjectFinder<UParticleSystem> particleAsset(
		TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_waterBall.P_ky_waterBall'"));
	if (particleAsset.Succeeded())
	{
		GetParticle()->SetTemplate(particleAsset.Object);
	}

	m_Particle->SetupAttachment(m_MeshComponent);

	//newScale = GetActorScale3D();

	//// ����� ���� ����
	//ConstructorHelpers::FObjectFinder<USoundBase> soundasset(TEXT("SoundWave'/Game/Interface_And_Item_Sounds/WAV/Bar_Filling_02.Bar_Filling_02'"));
	//if (soundasset.Succeeded())
	//{
	//	m_Sound = soundasset.Object;
	//}

	//ConstructorHelpers::FObjectFinder<USoundAttenuation> soundAtt(TEXT("SoundAttenuation'/Game/BlueprintClass/DefaulAttenuation.DefaulAttenuation'"));
	//if (soundAtt.Succeeded())
	//{
	//	m_Attenuation = soundAtt.Object;
	//}
}


// Called when the game starts or when spawned
void AKillerRCProjectile::BeginPlay() {
	Super::BeginPlay();

	//��� ����
	GetBox()->OnComponentHit.AddDynamic(this, &AKillerRCProjectile::OnHit);
	GetBox()->OnComponentBeginOverlap.AddDynamic(this, &AKillerRCProjectile::OnBeginOverlap);
}


// Called every frame
void AKillerRCProjectile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	m_fAccTime += DeltaTime;
	if (3.f <= m_fAccTime)
	{
		Destroy();
	}
}

void AKillerRCProjectile::OnHit(UPrimitiveComponent* _HitComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp
	, FVector _NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(_HitComponent, _OtherActor, _OtherComp, _NormalImpulse, Hit);
	
	//// �Ҹ� ���, 2D
	//UGameplayStatics::PlaySoundAtLocation(
		//GetWorld(), 
		//m_Sound, 
		//GetActorLocation(), 1.f, 1.f, 0.f, m_Attenuation);

	//floor && ĳ���� �ڽ� �浹 ���ֱ� (������ �ݸ��� ����)
	isHit = true;

	UE_LOG(LogTemp, Log, TEXT("%s"), *(_OtherActor->GetName()));

	//����Ʈ ���� & ���
	FTransform trans(GetActorRotation(), GetActorLocation() );
	UMyEffectManager::GetInst(GetWorld())->CreateEffect(EKillerEffect::PARTICLEHIT, trans, GetLevel());
	
	//ĳ���Ϳ��� ����������
	ASurvivor* survivor = Cast<ASurvivor>(_OtherActor);
	if (survivor != nullptr) { //cast ���н� null
		//�����ڰ� �°��ϱ�
		UGameplayStatics::ApplyDamage(survivor, 15.f, NULL, GetOwner(), NULL);
	}

	Destroy();

}

void AKillerRCProjectile::OnBeginOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex, bool _bFromSweep, const FHitResult& _SweepResult)
{

}

void AKillerRCProjectile::OnEndOverlap(UPrimitiveComponent* _PrimitiveComponent, AActor* _OtherActor, UPrimitiveComponent* _OtherComp, int32 _OtherBodyIndex)
{

}