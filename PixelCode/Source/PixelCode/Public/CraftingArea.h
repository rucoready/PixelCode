// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreateItemData.h"
#include "CraftingArea.generated.h"

class USphereComponent;
class UStaticMeshComponent;

enum class ECraftArea : uint8; 

UCLASS()
class PIXELCODE_API ACraftingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACraftingArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);




	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereOverlap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Crafts")
	ECraftArea CraftArea;


};
