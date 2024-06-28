#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Task_JumpAttack03.generated.h"

/**
 *
 */
UCLASS()
class PIXELCODE_API UTask_JumpAttack03 : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:

	explicit UTask_JumpAttack03(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	class UAnimMontage* jumpAttack03;

	FVector playerLocation;

	bool animOnce;

	float currentTime;

	bool jumpOnce = false;

	bool downOnce = false;

	bool niagaraOnce = false;

	UPROPERTY(EditAnywhere, Category = "Niagara")
	class UNiagaraSystem* groundImpacts;

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* groundImpactParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;
};