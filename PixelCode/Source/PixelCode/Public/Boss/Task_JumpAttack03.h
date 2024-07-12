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

	

	FVector playerLocation;

	

	bool animOnce;

	bool animOnceV2;

	bool animOnceV3;

	float currentTime;

	bool jumpOnce = false;

	bool downOnce = false;

	bool niagaraOnce = false;

	bool cameraMovingOnce = false;

	

	UPROPERTY(EditAnywhere, Category = "Particles")
	UParticleSystem* groundImpactParticleV2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings")
	TSubclassOf<UCameraShakeBase> cameraShakeOBJ;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector jumpAttack3CoolTime;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class APixelCodeCharacter* player;

	bool jumpAttack3;
};