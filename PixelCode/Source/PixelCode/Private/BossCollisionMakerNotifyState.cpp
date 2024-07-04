// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCollisionMakerNotifyState.h"
#include "Kismet/GameplayStatics.h"
#include "BossCollisionComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"



void UBossCollisionMakerNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    if (MeshComp)
    {
        // Skeletal Mesh Component에서 BossCollisionComponent를 찾습니다.
        UBossCollisionComponent* BossCollisionComp = MeshComp->GetOwner()->FindComponentByClass<UBossCollisionComponent>();
        if (BossCollisionComp)
        {
            // 충돌 감지 컴포넌트의 활성화 플래그를 true로 설정합니다.
            BossCollisionComp->SetEnableCollision(true);
        }
    }
}

void UBossCollisionMakerNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp)
    {
        // Skeletal Mesh Component에서 BossCollisionComponent를 찾습니다.
        UBossCollisionComponent* BossCollisionComp = MeshComp->GetOwner()->FindComponentByClass<UBossCollisionComponent>();
        if (BossCollisionComp)
        {
            // 충돌 감지 컴포넌트의 활성화 플래그를 false로 설정합니다.
            BossCollisionComp->SetEnableCollision(false);
        }
    }
}
