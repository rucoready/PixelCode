// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Pooling/PlayerObjectPoolManager.h"
#include "Player/PixelCodeCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"



// Sets default values
APlayerObjectPoolManager::APlayerObjectPoolManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MaxPoolSize = 5; // 기본 풀 크기 설정
}

// Called when the game starts or when spawned
void APlayerObjectPoolManager::BeginPlay()
{
	Super::BeginPlay(); // 부모 클래스의 BeginPlay 호출
	
    // 시작할 때 풀에 캐릭터 생성하여 추가
    for (int32 i = 0; i < MaxPoolSize; i++)
    {
        APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
        NewCharacter->SetActorHiddenInGame(true); // 게임에서 숨김
        NewCharacter->SetActorEnableCollision(false); // 충돌 비활성화
        NewCharacter->GetCharacterMovement()->Deactivate(); // 움직임 비활성화
        Pool.Add(NewCharacter); // 풀에 추가
    }

}

// Called every frame
void APlayerObjectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



APixelCodeCharacter* APlayerObjectPoolManager::GetPooledCharacter()
{
    // 사용 가능한 캐릭터 찾아 반환
    for (APixelCodeCharacter* Character : Pool)
    {
        if (Character->IsHidden()) // 숨겨진 캐릭터인지 확인
        {
            return Character;
        }
    }

    // 사용 가능한 캐릭터가 없으면 새로 생성하여 반환
    APixelCodeCharacter* NewCharacter = GetWorld()->SpawnActor<APixelCodeCharacter>(CharacterClass);
    Pool.Add(NewCharacter); // 풀에 추가
    return NewCharacter;
}

void APlayerObjectPoolManager::ReturnPooledCharacter(APixelCodeCharacter* PlayerChar)
{
    PlayerChar->SetActorHiddenInGame(true); // 숨김 처리
    PlayerChar->SetActorEnableCollision(false); // 충돌 비활성화
    PlayerChar->GetCharacterMovement()->Deactivate(); // 움직임 비활성화

}


