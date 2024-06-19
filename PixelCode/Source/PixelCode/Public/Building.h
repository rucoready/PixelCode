// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"



//class UInstancedStaticMeshComponent;

UCLASS()
class PIXELCODE_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = KSH)
	class UInstancedStaticMeshComponent* FoundationInstancedMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;









public:
	UFUNCTION(BlueprintCallable, Category = KSH)
	void DestroyInstance(FVector HitPoint);


	UFUNCTION(BlueprintCallable, Category = KSH)
	 FTransform GetInstancedSocketTransform(UInstancedStaticMeshComponent* InstancedComponent, int32 InstanceIndex, const FName& SocketName, bool& Success, bool WorldSpace = false);

};
