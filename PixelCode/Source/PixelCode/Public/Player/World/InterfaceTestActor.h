// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Interfaces/InteractionInterface.h"
#include "InterfaceTestActor.generated.h"

class UWidgetComponent;
//class UportalBarWidget;

UCLASS()
class PIXELCODE_API AInterfaceTestActor : public AActor, public IInteractionInterface // 인터페이스 상속
{
	GENERATED_BODY()
	
public:
	AInterfaceTestActor();

	virtual void Tick(float DeltaTime) override;

	class UInteractionWidget* InteractionWidget;

	/*UPROPERTY(EditAnywhere, Category = "Portal")
	TSubclassOf<class AportalActor> portalActorclass;*/
	

	bool bportalOnOff = false;
	float bportalTime = 0.0f;

	/*UPROPERTY(EditAnywhere, Category = "Portal")
	UWidgetComponent* portalBarWidgetComp;*/

	/*UPROPERTY(EditAnywhere, Category = "Portal")
	UportalBarWidget* portalBarWidget;*/

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundBase* portalSound;*/

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Test Actor")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Test Actor")
	FInteractableData InstanceInteractableData; // 상호작용 가능한 데이터의 인스턴스를 생성해야함.


	// 인터페이스가 선언하는 함수 재정의
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact(APixelCodeCharacter* PlayerCharacter) override;


};
