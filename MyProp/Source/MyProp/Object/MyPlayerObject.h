// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <EngineMinimal.h>
#include "../Player/MyCharacter.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyPlayerObject.generated.h"

UCLASS()
class MYPROP_API AMyPlayerObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyPlayerObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* m_Mesh;

	UStaticMeshComponent* GetMyMesh() { return m_Mesh; }

	//void ChangePlayerMesh(AActor* target, FKey ButtomPressed);//AddDynamic���� �߰����ֱ� ���ؼ� �μ��� �����־���
	
	//�÷��̾ ���� ��Ų�� (�������Ʈ���� �θ���)
	UFUNCTION(BlueprintCallable, Category = "PlayerObject")
		void ChangePlayerToObject();


};
