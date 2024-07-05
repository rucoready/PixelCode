// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossApernia.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/StaticMeshComponent.h"
#include "BossSword.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"


// Sets default values
ABossApernia::ABossApernia()
{
    PrimaryActorTick.bCanEverTick = true;

    //Set Character Mesh
    static ConstructorHelpers::FObjectFinder<USkeletalMesh>bossMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KMS_AI/Boss_Alpernia/BossAlpernia.BossAlpernia'"));
    if (bossMesh.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(bossMesh.Object);
    }
    GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
    GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

    //Set Character Anim Class
    static ConstructorHelpers::FClassFinder<UAnimInstance> bossAnimClass(TEXT("/Script/Engine.AnimBlueprint'/Game/KMS_AI/Boss_Alpernia/Animations/BlendSpace/BP_BossAlperniaAnimInstance.BP_BossAlperniaAnimInstance_C'"));
    if (bossAnimClass.Succeeded())
    {
        GetMesh()->SetAnimClass(bossAnimClass.Class);
    }


    // UChildActorComponent 생성
    bossSwordComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("bossSwordComp"));

    // 부모 메쉬에 Attach
    bossSwordComp->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));

    // FObjectFinder를 사용하여 ABossSword 클래스 객체를 찾습니다.
    static ConstructorHelpers::FClassFinder<ABossSword> bossSwordAsset(TEXT("/Script/Engine.Blueprint'/Game/KMS_AI/Boss_Alpernia/BossSword/BP_BossSword.BP_BossSword'_C"));
    if (bossSwordAsset.Succeeded())
    {
        // ABossSword 클래스를 설정합니다.
        bossSwordComp->SetChildActorClass(bossSwordAsset.Class);
    }

    // 위치, 회전, 스케일 설정
    bossSwordComp->SetRelativeLocation(FVector(29.122834f, 84.073564f, 4.113887f));
    bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
    bossSwordComp->SetWorldScale3D(FVector(1, 1, 1));
    bossSwordComp->SetVisibility(false);

    //back sword
    bossBackSwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("bossBackSwordComp"));
    //SwordComp Attach RightHandSocket!!
    bossBackSwordComp->SetupAttachment(GetMesh(), TEXT("SpineSocket"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh>bossBackSwordMesh(TEXT("/Script/Engine.StaticMesh'/Game/KMS_AI/Boss_Alpernia/BossSword/BossSword_Sword_low.BossSword_Sword_low'"));
    if (bossBackSwordMesh.Succeeded())
    {
        bossBackSwordComp->SetStaticMesh(bossBackSwordMesh.Object);
    }

    bossBackSwordComp->SetRelativeLocation(FVector(-28.945981f, -235.270012f, 29.204212f));
    bossBackSwordComp->SetRelativeRotation(FRotator(-0.016692f, 0.044553f, 20.539312f));
    bossBackSwordComp->SetWorldScale3D(FVector(0.7f, 0.5f, 1.0f));
    //bossBackSwordComp->SetVisibility(false);




}

// Called when the game starts or when spawned
void ABossApernia::BeginPlay()
{
    Super::BeginPlay();

    //UChildActorComponent* temp = GetComponentByClass<UChildActorComponent>(UChildActorComponent::StaticClass());

    

}

// Called every frame
void ABossApernia::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossApernia::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UBehaviorTree* ABossApernia::GetBehaviorTree() const
{
    return tree;
}

void ABossApernia::SetupStimulusSource()
{
    StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
    if (StimulusSource)
    {

        StimulusSource->RegisterForSense(TSubclassOf <UAISense_Sight>());
        StimulusSource->RegisterWithPerceptionSystem();

    }
}

void ABossApernia::SwordCollisionActive()
{


    // Get the child actor from the UChildActorComponent
    ABossSword* SwordActor = Cast<ABossSword>(bossSwordComp->GetChildActor());

    if (SwordActor)
    {
        // Assuming the collision component is named "damageSphereComp" in ABossSword
        UPrimitiveComponent* DamageSphereComp = SwordActor->FindComponentByClass<UPrimitiveComponent>();
        
        
        
        SwordActor->SwordCollisionActive();
        
        
    }
    else
    {
        
    }



}

void ABossApernia::SwordCollisionDeactive()
{


    // Get the child actor from the UChildActorComponent
    ABossSword* SwordActor = Cast<ABossSword>(bossSwordComp->GetChildActor());

    if (SwordActor)
    {
        // Assuming the collision component is named "damageSphereComp" in ABossSword
        UPrimitiveComponent* DamageSphereComp = SwordActor->FindComponentByClass<UPrimitiveComponent>();

        SwordActor->SwordCollisionDeactive();
        
    }
    else
    {
        
    }




}

