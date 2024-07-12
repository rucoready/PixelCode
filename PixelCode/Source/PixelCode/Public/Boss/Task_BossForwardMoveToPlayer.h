// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_BossForwardMoveToPlayer.generated.h"

/**
 *
 */
UCLASS()
class PIXELCODE_API UTask_BossForwardMoveToPlayer : public UBTTask_BlackboardBase
{
    GENERATED_BODY()

public:
    UTask_BossForwardMoveToPlayer(FObjectInitializer const& ObjectInitializer);

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(Replicated)
    float currentTime;

    float escapeTime;

    bool bFirstExecution = false;

    bool ComboAttack01Play = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    bool searchRandom = false;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    float searchRadius = 150.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings", Replicated)
    class UAnimMontage* swordComboAttack1;

    UPROPERTY(EditAnywhere, Category = "Niagara", Replicated)
    class UNiagaraSystem* impactSwing;

    UPROPERTY(EditAnywhere, Category = "Niagara", Replicated)
    class UNiagaraSystem* impactSwing2;

    UPROPERTY(Replicated)
    bool animOnce;

    UPROPERTY(Replicated)
    bool swingSwordNiagaraOnce = false;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector forwardSlashCoolTime;

    UPROPERTY(Replicated)
    bool forwardSlash;

    UPROPERTY(EditAnywhere, Category = "MySettings")
    class APixelCodeCharacter* player;

    bool excuteOnceFindPlayer = false;

    // 멤버 변수로 OwnerComp 저장
    UBehaviorTreeComponent* CachedOwnerComp;
    // 멤버 변수로 NodeMemory 저장
    uint8* CachedNodeMemory;
    // 멤버 변수로 DeltaSeconds 저장
    float CachedDeltaSeconds;

    //network

    void TaskDoIt(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);



    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // 서버에서 실행할 함수 선언
    UFUNCTION(Server, Reliable)
    void ServerRPC_PlayComboAttack();

    // 모든 클라이언트에게 실행할 함수 선언
    UFUNCTION(NetMulticast, Reliable)
    void MulticastRPC_PlayNiagaraEffects();


};
