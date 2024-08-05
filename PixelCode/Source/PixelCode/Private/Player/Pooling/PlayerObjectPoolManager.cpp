// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pooling/PlayerObjectPoolManager.h"
#include "Player/PixelCodeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
APlayerObjectPoolManager::APlayerObjectPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MaxPoolSize = 0; // �⺻ Ǯ ũ�� ����
}

// Called when the game starts or when spawned
void APlayerObjectPoolManager::BeginPlay()
{
	Super::BeginPlay(); // �θ� Ŭ������ BeginPlay ȣ��
	
    // ������ �� Ǯ�� ĳ���� �����Ͽ� �߰�
    for (int32 i = 0; i < MaxPoolSize; i++)
    {
        APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
        NewCharacter->SetActorHiddenInGame(true); // ���ӿ��� ����
        NewCharacter->SetActorEnableCollision(false); // �浹 ��Ȱ��ȭ
        NewCharacter->GetCharacterMovement()->Deactivate(); // ������ ��Ȱ��ȭ
        Pool.Add(NewCharacter); // Ǯ�� �߰�
    }

}

// Called every frame
void APlayerObjectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



APixelCodeCharacter* APlayerObjectPoolManager::GetPooledCharacter()
{
    // ��� ������ ĳ���� ã�� ��ȯ
    for (APixelCodeCharacter* Character : Pool)
    {
        if (Character->IsHidden()) // ������ ĳ�������� Ȯ��
        {
            return Character;
        }
    }

    // ��� ������ ĳ���Ͱ� ������ ���� �����Ͽ� ��ȯ
    APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
    Pool.Add(NewCharacter); // Ǯ�� �߰�
    return NewCharacter;
}

void APlayerObjectPoolManager::ReturnPooledCharacter(APixelCodeCharacter* PlayerChar)
{
    PlayerChar->SetActorHiddenInGame(true); // ���� ó��
    PlayerChar->SetActorEnableCollision(false); // �浹 ��Ȱ��ȭ
    PlayerChar->GetCharacterMovement()->Deactivate(); // ������ ��Ȱ��ȭ

}


