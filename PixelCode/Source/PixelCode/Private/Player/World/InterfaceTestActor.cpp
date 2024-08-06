// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/World/InterfaceTestActor.h"
#include "Components/StaticMeshComponent.h"
//#include "portal/portalActor.h"
#include "Player/Interfaces/InteractionWidget.h"
//#include "portal/portalBarWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>


// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	/*portalBarWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("portalComp"));
	portalBarWidgetComp->SetupAttachment(RootComponent);*/




}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData; // 인스턴스 데이터와 같게설정

	InteractionWidget = Cast<UInteractionWidget>(InteractionWidget);

	/*portalBarWidget = Cast<UportalBarWidget>(portalBarWidgetComp->GetWidget());

	portalBarWidget->SetVisibility(ESlateVisibility::Collapsed);*/

}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (bportalOnOff == true)
	//{
	//	bportalTime += DeltaTime;
	//	//UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on Interface test actor")); // 시작 상호작용 재정의 호출
	//	if (bportalTime >= 8.0f)
	//	{
			/*if (portalSound)
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), portalSound, GetActorLocation());
			}*/


			/*	FActorSpawnParameters params;
				params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				GetWorld()->SpawnActor<AportalActor>(portalActorclass, GetActorLocation() + FVector(0, 0, 100), GetActorRotation(), params);
				bportalTime = 0;
				bportalOnOff = false;
				Destroy();
			}
			if (portalBarWidget != nullptr)
			{
				portalBarWidget->portalBarUpdate(bportalTime);
			}
		}
		else
		{
			bportalTime = 0;
		}*/


}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true); // 정의깊이 랜더링 부울 입력값 설정

	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false); // 정의깊이 랜더링 부울 입력값 설정
	}
}

void AInterfaceTestActor::BeginInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling BeginInteract override on Interface test actor")); // 시작 상호작용 재정의 호출


	//bportalOnOff = true;
	//portalBarWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInterfaceTestActor::EndInteract()
{
	//bportalOnOff = false;
	//UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on Interface test actor"));
	//portalBarWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AInterfaceTestActor::Interact(APixelCodeCharacter* PlayerCharacter)
{
	//UE_LOG(LogTemp, Warning, TEXT("Calling Interact override on Interface test actor"));

}

