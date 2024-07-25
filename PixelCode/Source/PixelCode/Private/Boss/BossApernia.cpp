// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossApernia.h"
#include "Boss/BossAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Components/StaticMeshComponent.h"
#include "BossSword.h"
#include "Player/PixelCodeCharacter.h"
#include "Components/ChildActorComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMesh.h"
#include <../../../../../../../Source/Runtime/Engine/Public/Net/UnrealNetwork.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Player/PlayerOrganism.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Sound/SoundBase.h" 
#include "GameFramework/CharacterMovementComponent.h"
#include "BossMaInUI.h"
#include "Blueprint/UserWidget.h"
#include "Boss/BossAnimInstance.h"
#include "DamageWidget.h"
#include "DamageWidgetComponent.h"
#include "Components/WidgetComponent.h"


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

    damageWidgetComponentl = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageWidgetComponent"));
    damageWidgetComponentl->SetupAttachment(GetMesh());  // Attach to the owner's root component
    damageWidgetComponentl->SetRelativeLocation(FVector(0, 0, 188));
    damageWidgetComponentl->SetRelativeRotation(FRotator(0, -99, 0));

    // Optionally, you can set a widget class to the DamageWidgetComponent
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/Engine.Blueprint'/Game/KMS_AI/Damage/BP_DamageWidgetComponent.BP_DamageWidgetComponent'"));
    if (WidgetClass.Succeeded())
    {
        damageWidgetComponentl->SetWidgetClass(WidgetClass.Class);
    }

    //////////////////////////////////////////////////////////////////////////Boss Animation forward slash
    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack01.AM_BossComboAttack01'"));
    if (MontageObj.Succeeded())
    {
        swordComboAttack1 = MontageObj.Object;
    }
    //Jump Attack01
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj2(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack02_Montage.AM_BossComboAttack02_Montage'"));
    if (montageObj2.Succeeded())
    {
        jumpAttack01V1 = montageObj2.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj3(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z1_Montage.AM_Z1_Montage'"));
    if (montageObj3.Succeeded())
    {
        jumpAttack01V2 = montageObj3.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj4(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z2_Montage.AM_Z2_Montage'"));
    if (montageObj4.Succeeded())
    {
        jumpAttack01V3 = montageObj4.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj5(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_Z3_Montage.AM_Z3_Montage'"));
    if (montageObj5.Succeeded())
    {
        jumpAttack01V4 = montageObj5.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj6(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossJumpAttack01.AM_BossJumpAttack01'"));
    if (montageObj6.Succeeded())
    {
        jumpAttack02V1 = montageObj6.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj7(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalSlashV5_Montage.AM_NormalSlashV5_Montage'"));
    if (montageObj7.Succeeded())
    {
        jumpAttack02V2 = montageObj7.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj8(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_SlashV6_Montage.AM_SlashV6_Montage'"));
    if (montageObj8.Succeeded())
    {
        jumpAttack03V1 = montageObj8.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj9(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_SlashV7_Montage.AM_SlashV7_Montage'"));
    if (montageObj9.Succeeded())
    {
        jumpAttack03V2 = montageObj9.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj10(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalAttack03V4_Montage.AM_NormalAttack03V4_Montage'"));
    if (montageObj10.Succeeded())
    {
        jumpAttack03V3 = montageObj10.Object;
    }
    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj11(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AS_BossNormalAttack01.AS_BossNormalAttack01'"));
    if (montageObj11.Succeeded())
    {
        swordNormalAttack01V1 = montageObj11.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj12(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalAttackV2_Montage.AM_NormalAttackV2_Montage'"));
    if (montageObj12.Succeeded())
    {
        swordNormalAttack01V2 = montageObj12.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj13(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AS_BossLongSwing_Montage.AS_BossLongSwing_Montage'"));
    if (montageObj13.Succeeded())
    {
        swordNormalAttack02V1 = montageObj13.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj14(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationV2/AM_NormalAttack03V4_Montage.AM_NormalAttack03V4_Montage'"));
    if (montageObj14.Succeeded())
    {
        swordNormalAttack02V2 = montageObj14.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj15(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_NormalSlash04_Montage.AM_NormalSlash04_Montage'"));
    if (MontageObj15.Succeeded())
    {
        swordNormalAttack04 = MontageObj15.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj16(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AM_BossComboAttack01.AM_BossComboAttack01'"));
    if (MontageObj16.Succeeded())
    {
        forwardSlashAttack = MontageObj16.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj17(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_BackAttackDodge01_Montage.AM_BackAttackDodge01_Montage'"));
    if (montageObj17.Succeeded())
    {
        dodgeAndAttack01 = montageObj17.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj18(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AM_DoubleSwing_Montage.AM_DoubleSwing_Montage'"));
    if (montageObj18.Succeeded())
    {
        doubleSwingAttack = montageObj18.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj19(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_Dodge01_Montage.AS_Dodge01_Montage'"));
    if (montageObj19.Succeeded())
    {
        dodge01 = montageObj19.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj20(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_Dodge02_Montage.AS_Dodge02_Montage'"));
    if (montageObj20.Succeeded())
    {
        dodge02 = montageObj20.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj21(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_DodgeBack_Montage.AS_DodgeBack_Montage'"));
    if (montageObj21.Succeeded())
    {
        dodge03 = montageObj21.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj22(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_BossLongAttack01_Montage.AS_BossLongAttack01_Montage'"));
    if (montageObj22.Succeeded())
    {
        stingAttack01 = montageObj22.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj23(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AS_FarJumpAttack.AS_FarJumpAttack'"));
    if (montageObj23.Succeeded())
    {
        farStompAttack = montageObj23.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj24(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationFinish/AS_BossEquitSword_Montage.AS_BossEquitSword_Montage'"));
    if (montageObj24.Succeeded())
    {
        swordEquipMT = montageObj24.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> montageObj25(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationDrop/AM_BossDownAndUp.AM_BossDownAndUp'"));
    if (montageObj25.Succeeded())
    {
        bossFallDownMT = montageObj25.Object;
    }

    static ConstructorHelpers::FObjectFinder<UAnimMontage> MontageObj26(TEXT("/Script/Engine.AnimMontage'/Game/KMS_AI/Boss_Alpernia/Animations/AnimationCounter/AS_BossCounterDeclare_Montage.AS_BossCounterDeclare_Montage'"));
    if (MontageObj26.Succeeded())
    {
        counterPrecursor = MontageObj26.Object;
    }
    ////////////////NIAGARA////////////////////////
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj1(TEXT("/Script/Niagara.NiagaraSystem'/Game/NiagaraMagicalSlashes/Fx/Slashes/NS_Blade_Sl_10.NS_Blade_Sl_10'"));
    if (niagaraObj1.Succeeded())
    {
        groundImpactV1Niagara = niagaraObj1.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj2(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Slash.NA_Slash'"));
    if (niagaraObj2.Succeeded())
    {
        jumpSwing = niagaraObj2.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj3(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Slash.NA_Slash'"));
    if (niagaraObj3.Succeeded())
    {
        jumpSwing2 = niagaraObj3.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj4(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Sting.NA_Sting'"));
    if (niagaraObj4.Succeeded())
    {
        jumpSwing3 = niagaraObj4.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj5(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_SwordSmashing03.NA_SwordSmashing03'"));
    if (niagaraObj5.Succeeded())
    {
        impactSwing = niagaraObj5.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj6(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NS_Claw2.NS_Claw2'"));
    if (niagaraObj6.Succeeded())
    {
        doubleSwingNA2 = niagaraObj6.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj7(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Sting.NA_Sting'"));
    if (niagaraObj7.Succeeded())
    {
        doubleSwingV2 = niagaraObj7.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj8(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Sting.NA_Sting'"));
    if (niagaraObj8.Succeeded())
    {
        doubleSwingV3 = niagaraObj8.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj9(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_Sting.NA_Sting'"));
    if (niagaraObj9.Succeeded())
    {
        stingAttackNiagara = niagaraObj9.Object;
    }

    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraObj10(TEXT("/Script/Niagara.NiagaraSystem'/Game/KMS_AI/Boss_Alpernia/Effects/NA_GroundAttack01.NA_GroundAttack01'"));
    if (niagaraObj10.Succeeded())
    {
        groundImpactNiagara2 = niagaraObj10.Object;
    }
    



    
    ////////////////Particle////////////////////////
    static ConstructorHelpers::FObjectFinder<UParticleSystem> particleObj1(TEXT("/Script/Engine.ParticleSystem'/Game/KMS_AI/Boss_Alpernia/Effects/P_GroundImpact02.P_GroundImpact02'"));
    if (particleObj1.Succeeded())
    {
        groundImpactParticle = particleObj1.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> particleObj2(TEXT("/Script/Engine.ParticleSystem'/Game/KMS_AI/Boss_Alpernia/Effects/P_GroundImpact01.P_GroundImpact01'"));
    if (particleObj2.Succeeded())
    {
        groundImpactParticle2 = particleObj2.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> particleObj3(TEXT("/Script/Engine.ParticleSystem'/Game/KMS_AI/Boss_Alpernia/Effects/CounterHit.CounterHit'"));
    if (particleObj3.Succeeded())
    {
        counterImpactParticle = particleObj3.Object;
    }

    ////////////////Camera////////////////////////
    static ConstructorHelpers::FClassFinder<UCameraShakeBase> cameraObj1(TEXT("/Script/Engine.Blueprint'/Game/KMS_AI/Boss_Alpernia/CameraShake/BP_CameraShakeStomp.BP_CameraShakeStomp'_C"));
    if (cameraObj1.Succeeded())
    {
        cameraShakeOBJ = cameraObj1.Class;
    }


    //Material
    
    static ConstructorHelpers::FObjectFinder<UMaterial> OriginalMaterialFinder(TEXT("/Script/Engine.Material'/Game/KMS_AI/Boss_Alpernia/M_Alpernia/mn_ldea_01_mi.mn_ldea_01_mi'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> OriginalMaterialFinder2(TEXT("/Script/Engine.Material'/Game/KMS_AI/Boss_Alpernia/M_Alpernia/mn_ldea_01-1_mi.mn_ldea_01-1_mi'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> DamageMaterialFinder(TEXT("/Script/Engine.Material'/Game/KMS_AI/Boss_Alpernia/M_Alpernia/TestBoss/mn_ldea_01-1_m222222222.mn_ldea_01-1_m222222222'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> counterMaterialFinder(TEXT("/Script/Engine.Material'/Game/KMS_AI/Boss_Alpernia/M_Alpernia/TestBoss/mn_ldea_01_Counter.mn_ldea_01_Counter'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> counterMaterial2Finder(TEXT("/Script/Engine.Material'/Game/KMS_AI/Boss_Alpernia/M_Alpernia/TestBoss/mn_ldea_01-1_Counter.mn_ldea_01-1_Counter'"));
    if (OriginalMaterialFinder.Succeeded())
    {
        originalMaterial = OriginalMaterialFinder.Object;
    }
    if (OriginalMaterialFinder2.Succeeded())
    {
        originalMaterial = OriginalMaterialFinder.Object;
    }
    if (DamageMaterialFinder.Succeeded())
    {
        damageMaterial = DamageMaterialFinder.Object;
    }
    if (counterMaterialFinder.Succeeded())
    {
        counterMaterial = counterMaterialFinder.Object;
    }
    if (counterMaterial2Finder.Succeeded())
    {
        counterMaterial2 = counterMaterial2Finder.Object;
    }


}

// Called when the game starts or when spawned
void ABossApernia::BeginPlay()
{
    Super::BeginPlay();

    //set boss maxHP
    bossCurrentHP = bossMaxHP;
    InitMainUI();
}

// Called every frame
void ABossApernia::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    

    if (bossCurrentHP <= 0.0f&&!bossOnceDie)
    {
        bossOnceDie = true;
        ServerRPC_BossDie();
        if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
        {
            if (UBehaviorTreeComponent* BTComponent = bossController->FindComponentByClass<UBehaviorTreeComponent>())
            {
                savedBTComponent = BTComponent;
                savedLocation = GetActorLocation();
                
                bossDied = true;
                
                bossController->ClearFocus(EAIFocusPriority::Default);
                
                BTComponent->DestroyComponent();
                bossController->StopMovement();
                FRotator currentRotation = GetActorRotation();
                SetActorRotation(currentRotation);
                //BTComponent->StopTree(EBTStopMode::Safe);
                GetWorldTimerManager().SetTimer(timerhandle_Destroy, this, &ABossApernia::DestroySelf, 4.2f, false);
            }
        }
        
    }
    

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
    ABossSword* SwordActor = Cast<ABossSword>(bossSwordComp->GetChildActor());

    if (SwordActor)
    {
        UPrimitiveComponent* DamageSphereComp = SwordActor->FindComponentByClass<UPrimitiveComponent>();  
        SwordActor->SwordCollisionActive();
    }

}

void ABossApernia::SwordCollisionDeactive()
{

    ABossSword* SwordActor = Cast<ABossSword>(bossSwordComp->GetChildActor());

    if (SwordActor)
    {
        UPrimitiveComponent* DamageSphereComp = SwordActor->FindComponentByClass<UPrimitiveComponent>();

        SwordActor->SwordCollisionDeactive();
        
    }

}

void ABossApernia::DestroySelf()
{
    Destroy();
}

void ABossApernia::BossFallDownReset()
{
    bBossAttackFallDownAttack = false;
}

void ABossApernia::SetOriginMaterial()
{
    if (originalMaterial)
    {
        USkeletalMeshComponent* MeshComp = GetMesh(); // SkeletalMeshComponent를 가져옵니다.
        if (MeshComp)
        {
            // 기존에 있는 머티리얼 슬롯 인덱스를 확인하고 적절히 설정합니다.
            int32 MaterialIndex = 0; // 적절한 슬롯 인덱스 지정
            int32 MaterialIndex2 = 1; // 적절한 슬롯 인덱스 지정
            MeshComp->SetMaterial(MaterialIndex, originalMaterial);
            MeshComp->SetMaterial(MaterialIndex2, originalMaterial);
        }
    }
}

void ABossApernia::SetCounterMaterial()
{
    // Material 변경
    if (damageMaterial)
    {
        USkeletalMeshComponent* MeshComp = GetMesh(); // SkeletalMeshComponent를 가져옵니다.
        if (MeshComp)
        {
            // 기존에 있는 머티리얼 슬롯 인덱스를 확인하고 적절히 설정합니다.
            int32 MaterialIndex = 0; // 적절한 슬롯 인덱스 지정
            int32 MaterialIndex2 = 1; // 적절한 슬롯 인덱스 지정
            MeshComp->SetMaterial(MaterialIndex, counterMaterial);
            MeshComp->SetMaterial(MaterialIndex2, counterMaterial2);


        }
    }
}

void ABossApernia::ReflagCounterAttack()
{
    bossHitCounterAttack = false;
    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
    if (animInstance)
    {
        // BossAnimInstance 타입으로 캐스팅합니다.
        UBossAnimInstance* bossInstance = Cast<UBossAnimInstance>(animInstance);
        if (bossInstance)
        {
            bossInstance->canCounterAttack = false;
        }
    }
}

void ABossApernia::BossTakeDamage(float Damage)
{
    bossCurrentHP = bossCurrentHP - Damage;
    Player = Cast<APlayerOrganism>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
    
    bossMainWidget->UpdateHPBar(bossCurrentHP);

    ServerRPC_BossTakeDamageWidgetSet();

    // 애니메이션 인스턴스를 가져옵니다.
    UAnimInstance* animInstance = GetMesh()->GetAnimInstance();
    if (animInstance)
    {
        // BossAnimInstance 타입으로 캐스팅합니다.
        UBossAnimInstance* bossInstance = Cast<UBossAnimInstance>(animInstance);
        if (bossInstance)
        {
            if (bossInstance->canCounterAttack == true)
            {
                bossBackSwordComp->SetRelativeLocation(FVector(-28.945981f, -235.270012f, 29.204212f));
                bossBackSwordComp->SetRelativeRotation(FRotator(-0.016692f, 0.044553f, 20.539312f));
                bossHitCounterAttack = true;
                bossInstance->canCounterAttack = false;
                PlayAnimMontage(counterGroggy);
                TaskCheckCounterHit = true;
                TArray<AActor*> foundCharacters;
                UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);


                


                for (AActor* actor : foundCharacters)
                {
                    APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
                    if (player)
                    {
                        APlayerController* pc = player->GetController<APlayerController>();
                        if (pc != nullptr)
                        {
                            UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                            pc->ClientStartCameraShake(cameraShakeCounterOBJ);
                            FVector ForwardVector = GetActorForwardVector();  // 현재 액터의 forwardVector 가져오기
               
                            float PulseStrength = 5200.0f;  // 펄스의 세기 

                            ServerRPC_CounterPrecursorSpawnParticle();

                            FVector LaunchDirection = -GetActorForwardVector(); // 뒷 방향으로 설정
                            LaunchCharacter(LaunchDirection * PulseStrength, true, true);
                            
                        }
                    }
                }




                bossInstance->AnimNotify_CollisionOff();
                GetWorldTimerManager().SetTimer(timerhandle_ReflagCounterAttack, this, &ABossApernia::ReflagCounterAttack, 7.f, false);

                if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
                {
                    bossController->ClearFocus(EAIFocusPriority::Default);
                    bossController->StopMovement();
                    FRotator currentRotation = GetActorRotation();
                    SetActorRotation(currentRotation);


                    if (UBehaviorTreeComponent* BTComponent = bossController->FindComponentByClass<UBehaviorTreeComponent>())
                    {
                        savedBTComponent = BTComponent;
                        savedLocation = GetActorLocation(); 
                        BTComponent->StopTree(EBTStopMode::Safe);
                    }
                    GetWorldTimerManager().SetTimer(timerhandle_RepocessBehaviorTree, this, &ABossApernia::RepocessBehaviorTreeRe, 5.0f, false);

                }
            }
        }
    }
    // Material 변경
    if (damageMaterial)
    {
        USkeletalMeshComponent* MeshComp = GetMesh(); // SkeletalMeshComponent를 가져옵니다.
        if (MeshComp)
        {
            // 기존에 있는 머티리얼 슬롯 인덱스를 확인하고 적절히 설정합니다.
            int32 MaterialIndex = 0; // 적절한 슬롯 인덱스 지정
            int32 MaterialIndex2 = 1; // 적절한 슬롯 인덱스 지정
            MeshComp->SetMaterial(MaterialIndex, damageMaterial);
            MeshComp->SetMaterial(MaterialIndex2, damageMaterial);

            GetWorldTimerManager().SetTimer(timerhandle_SetOriginMatetrial, this, &ABossApernia::SetOriginMaterial, 0.38f, false);
        }

        TArray<AActor*> foundCharacters;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

        for (AActor* actor : foundCharacters)
        {
            APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
            if (player)
            {
                APlayerController* pc = player->GetController<APlayerController>();
                if (pc != nullptr)
                {
                    UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                    pc->ClientStartCameraShake(cameraShakeHitPlayerOBJ);


                }
            }
        }
    }

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bissHitNA, GetActorLocation(), GetActorRotation(), FVector(5.0f));
    
}

void ABossApernia::BossFallDown()
{
    PlayAnimMontage(bossFallDownMT);
    if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
    {
        bossController->ClearFocus(EAIFocusPriority::Default);
        bossController->StopMovement();
        FRotator currentRotation = GetActorRotation();
        SetActorRotation(currentRotation);
        

        if (UBehaviorTreeComponent* BTComponent = bossController->FindComponentByClass<UBehaviorTreeComponent>())
        {
            savedBTComponent = BTComponent; // Save the BTComponent
            savedLocation = GetActorLocation(); // Save the current location
            BTComponent->StopTree(EBTStopMode::Safe);
        }
        GetWorldTimerManager().SetTimer(timerhandle_RepocessBehaviorTree, this, &ABossApernia::RepocessBehaviorTree, 6.0f, false);
        bBossAttackFallDownAttack = true;
    }
}

void ABossApernia::RepocessBehaviorTree()
{ 
    UE_LOG(LogTemp, Warning, TEXT("RepocessBehaviorTree called"));

    if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
    {
        if (savedBTComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("Restarting Behavior Tree"));
            bossController->BrainComponent->RestartLogic();
            SetActorLocation(savedLocation); // Restore the saved location if necessary
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Saved BT Component is null"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Boss AI Controller not found"));
    }
    
}

void ABossApernia::RepocessBehaviorTreeRe()
{
    UE_LOG(LogTemp, Warning, TEXT("RepocessBehaviorTreeRe called"));

    if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
    {
        if (savedBTComponent && tree)
        {
            UE_LOG(LogTemp, Warning, TEXT("Restarting Behavior Tree from the beginning"));
            bossController->BrainComponent->RestartLogic();
            bossController->RunBehaviorTree(tree);
        }
       
    }
    
}

void ABossApernia::ServerRPC_JumpAttack01V2_Implementation()
{
    MulticastRPC_JumpAttack01V2();
}

void ABossApernia::MulticastRPC_JumpAttack01V2_Implementation()
{
    PlayAnimMontage(jumpAttack01V2);
}

void ABossApernia::ServerRPC_JumpAttack01V1_Implementation()
{
    MulticastRPC_JumpAttack01V1();
}

void ABossApernia::MulticastRPC_JumpAttack01V1_Implementation()
{
    PlayAnimMontage(jumpAttack01V1);
}

//if (UBehaviorTreeComponent* BTComponent = bossController->FindComponentByClass<UBehaviorTreeComponent>())
//{
    // Behavior Tree를 다시 실행하기 위해 로컬 변수로 선언
   // UBehaviorTree* BehaviorTree = GetBehaviorTree();
    //if (BehaviorTree)
    //{
     //   BTComponent->StartTree(*BehaviorTree);
    //}
//}







void ABossApernia::InitMainUI()
{
    if (MainUIFactory)
    {
       bossMainWidget = CreateWidget<UBossMaInUI>(GetWorld(), MainUIFactory);
       if (bossMainWidget)
       {
           bossMainWidget->AddToViewport();
       }
    }
    bossMainWidget->UpdateHPBar(bossCurrentHP);
}

void ABossApernia::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABossApernia, swordComboAttack1);
    DOREPLIFETIME(ABossApernia, jumpAttack01V1);
    DOREPLIFETIME(ABossApernia, Player);
}

void ABossApernia::ServerRPC_ForwardSlash_Implementation()
{
    MulticastRPC_ForwardSlash();
}

void ABossApernia::MulticastRPC_ForwardSlash_Implementation()
{
    PlayAnimMontage(swordComboAttack1);
}

void ABossApernia::ServerRPC_JumpAttack01V3_Implementation()
{
    MulticastRPC_JumpAttack01V3();
}

void ABossApernia::MulticastRPC_JumpAttack01V3_Implementation()
{
    PlayAnimMontage(jumpAttack01V3);
}

void ABossApernia::ServerRPC_JumpAttack01V4_Implementation()
{
    MulticastRPC_JumpAttack01V4();
}

void ABossApernia::MulticastRPC_JumpAttack01V4_Implementation()
{
    PlayAnimMontage(jumpAttack01V4);
}

void ABossApernia::ServerRPC_SpawnJumpAttackNiagara_Implementation()
{
    MulticastRPC_SpawnJumpAttackNiagara();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackNiagara_Implementation()
{
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, -100.0f);
    FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, GetSimpleCollisionHalfHeight());
    FVector bossGroundLocation2 = bossLocation - FVector(0.0f, 0.0f, GetSimpleCollisionHalfHeight());
    FVector bossGroundLocation3 = bossLocation - FVector(0.0f, 0.0f, GetSimpleCollisionHalfHeight());

    FVector spawnLocation = bossGroundLocation + bossForwardVector * 200.0f;

    // -60도 회전 계산
    FRotator rotationMinus60 = GetActorRotation();
    rotationMinus60.Yaw -= 15.0f;
    FVector directionMinus60 = rotationMinus60.Vector();
    FVector spawnLocation2 = bossGroundLocation2 + directionMinus60 * 200.0f;

    // +60도 회전 계산
    FRotator rotationPlus60 = GetActorRotation();
    rotationPlus60.Yaw += 15.0f;
    FVector directionPlus60 = rotationPlus60.Vector();
    FVector spawnLocation3 = bossGroundLocation3 + directionPlus60 * 200.0f;

    // 현재 보스의 회전값을 가져옵니다.
    FRotator bossRotation = GetActorRotation();

    // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
    float newPitch = bossRotation.Pitch + 90.f;

    // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
    float roll = bossRotation.Roll;
    float yaw = bossRotation.Yaw;

    // 새로운 회전값을 FRotator로 설정합니다.
    FRotator bossSmashingRotator(newPitch, yaw, roll);

    bossSmashingRotator.Yaw += -90.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV1Niagara, spawnLocation, GetActorRotation(), FVector(2.0f));
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV1Niagara, spawnLocation2, rotationMinus60, FVector(2.0f));
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactV1Niagara, spawnLocation3, rotationPlus60, FVector(2.0f));
}

void ABossApernia::ServerRPC_JumpAttack02V1_Implementation()
{
    MulticastRPC_JumpAttack02V1();
}

void ABossApernia::MulticastRPC_JumpAttack02V1_Implementation()
{
    PlayAnimMontage(jumpAttack02V1);
}

void ABossApernia::ServerRPC_JumpAttack02V2_Implementation()
{
    MulticastRPC_JumpAttack02V2();
}

void ABossApernia::MulticastRPC_JumpAttack02V2_Implementation()
{
    PlayAnimMontage(jumpAttack02V2);
}

void ABossApernia::ServerRPC_SpawnJumpAttackNiagara2V1_Implementation()
{
    MulticastRPC_SpawnJumpAttackNiagara2V1();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackNiagara2V1_Implementation()
{

    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 500.0f);

    // 현재 보스의 회전값을 가져옵니다.
    FRotator bossRotation = GetActorRotation();

    // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
    float newPitch = bossRotation.Pitch + 270.f;

    // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
    float roll = bossRotation.Roll;
    float yaw = bossRotation.Yaw;

    // 새로운 회전값을 FRotator로 설정합니다.
    FRotator bossSmashingRotator(newPitch, yaw, roll);

    bossSmashingRotator.Yaw += -90.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing2, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
}

void ABossApernia::ServerRPC_SpawnJumpAttackNiagara2V2_Implementation()
{
    MulticastRPC_SpawnJumpAttackNiagara2V2();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackNiagara2V2_Implementation()
{
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 500.0f);

    // 현재 보스의 회전값을 가져옵니다.
    FRotator bossRotation = GetActorRotation();

    // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
    float newPitch = bossRotation.Pitch + -180.f;

    // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
    float roll = bossRotation.Roll;
    float yaw = bossRotation.Yaw;

    // 새로운 회전값을 FRotator로 설정합니다.
    FRotator bossSmashingRotator(newPitch, yaw, roll);

    bossSmashingRotator.Yaw += -90.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing3, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
}

void ABossApernia::ServerRPC_SpawnJumpAttackNiagara2V3_Implementation()
{
    MulticastRPC_SpawnJumpAttackNiagara2V3();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackNiagara2V3_Implementation()
{
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossSmashingLocation = bossLocation - FVector(0.0f, 0.0f, 200.0f);

    // 현재 보스의 회전값을 가져옵니다.
    FRotator bossRotation = GetActorRotation();

    // 현재 Pitch 축 값에 90도를 더한 회전값을 계산합니다.
    float newPitch = bossRotation.Pitch + -180.f;

    // Roll과 Yaw 축 값은 변하지 않도록 현재 값으로 설정합니다.
    float roll = bossRotation.Roll;
    float yaw = bossRotation.Yaw;

    // 새로운 회전값을 FRotator로 설정합니다.
    FRotator bossSmashingRotator(newPitch, yaw, roll);

    bossSmashingRotator.Yaw += -90.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), jumpSwing3, bossSmashingLocation, bossSmashingRotator, FVector(3.0f));
}

void ABossApernia::ServerRPC_JumpAttack03V1_Implementation()
{
    MulticastRPC_JumpAttack03V1();
}

void ABossApernia::MulticastRPC_JumpAttack03V1_Implementation()
{
    PlayAnimMontage(jumpAttack03V1);
}

void ABossApernia::ServerRPC_JumpAttack03V2_Implementation()
{
    MulticastRPC_JumpAttack03V2();
}

void ABossApernia::MulticastRPC_JumpAttack03V2_Implementation()
{
    PlayAnimMontage(jumpAttack03V2);
}

void ABossApernia::ServerRPC_JumpAttack03V3_Implementation()
{
    MulticastRPC_JumpAttack03V3();
}

void ABossApernia::MulticastRPC_JumpAttack03V3_Implementation()
{
    PlayAnimMontage(jumpAttack03V3);
}

void ABossApernia::ServerRPC_SpawnJumpAttackNiagara3V1_Implementation()
{
    MulticastRPC_SpawnJumpAttackNiagara3V1();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackNiagara3V1_Implementation()
{
    FVector bossLocation = GetActorLocation();

    FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, GetSimpleCollisionHalfHeight());

    FVector bossForwardVector = GetActorForwardVector();

    // 파티클을 스폰할 위치 계산
    FVector spawnLocation = bossGroundLocation + bossForwardVector * 400.0f;

    // 파티클 시스템을 월드에 스폰
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticle, spawnLocation, FRotator::ZeroRotator, FVector(1.0f, 1.0f, 1.0f));
}

void ABossApernia::ServerRPC_JumpAttack03CameraShake_Implementation()
{
    MulticastRPC_JumpAttack03CameraShake();
}

void ABossApernia::MulticastRPC_JumpAttack03CameraShake_Implementation()
{
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    for (AActor* actor : foundCharacters)
    {
        APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
        if (player)
        {
            APlayerController* pc = player->GetController<APlayerController>();
            if (pc != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                pc->ClientStartCameraShake(cameraShakeOBJ);


            }
        }
    }
}

void ABossApernia::ServerRPC_NormalAttack01V1_Implementation()
{
    MulticastRPC_NormalAttack01V1();
}

void ABossApernia::MulticastRPC_NormalAttack01V1_Implementation()
{
    PlayAnimMontage(swordNormalAttack01V1);
}

void ABossApernia::ServerRPC_NormalAttack01V2_Implementation()
{
    MulticastRPC_NormalAttack01V2();
}

void ABossApernia::MulticastRPC_NormalAttack01V2_Implementation()
{
    PlayAnimMontage(swordNormalAttack01V2);
}

void ABossApernia::ServerRPC_NormalAttack02V1_Implementation()
{
    MulticastRPC_NormalAttack02V1();
}

void ABossApernia::MulticastRPC_NormalAttack02V1_Implementation()
{
    PlayAnimMontage(swordNormalAttack02V1);
}

void ABossApernia::ServerRPC_NormalAttack02V2_Implementation()
{
    MulticastRPC_NormalAttack02V2();
}

void ABossApernia::MulticastRPC_NormalAttack02V2_Implementation()
{
    PlayAnimMontage(swordNormalAttack02V2);
}

void ABossApernia::ServerRPC_NormalAttack03V1_Implementation()
{
    MulticastRPC_NormalAttack03V1();
}

void ABossApernia::MulticastRPC_NormalAttack03V1_Implementation()
{
    PlayAnimMontage(swordNormalAttack04);
}

void ABossApernia::ServerRPC_ForwardSlashAttack_Implementation()
{
    MulticastRPC_ForwardSlasAttack();
}

void ABossApernia::MulticastRPC_ForwardSlasAttack_Implementation()
{
    PlayAnimMontage(forwardSlashAttack);
}

void ABossApernia::ServerRPC_SpawnNiagaraForwardSlash_Implementation()
{
    MulticastRPC_SpawnNiagaraForwardSlash();
}

void ABossApernia::MulticastRPC_SpawnNiagaraForwardSlash_Implementation()
{
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossSmashingLocation = bossLocation + FVector(0.0f, 0.0f, 0.0f);
    FVector bossSmashingLocationUpper = bossLocation - FVector(0.0f, 0.0f, 0.0f);

    // 보스의 회전 값을 가져옵니다.
    FRotator bossRotation = GetActorRotation();

    // bossRotation에서 Roll 값에 90도를 더한 새로운 회전 값을 생성합니다.
    FRotator UpperRotation = bossRotation;
    UpperRotation.Roll += 90.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactSwing, bossSmashingLocation, GetActorRotation(), FVector(3.0f));
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), impactSwing2, bossSmashingLocationUpper, UpperRotation, FVector(2.0f));
    
}

void ABossApernia::ServerRPC_DodgeAndSlashAttack_Implementation()
{
    MulticastRPC_DodgeAndSlashAttack();
}

void ABossApernia::MulticastRPC_DodgeAndSlashAttack_Implementation()
{
    PlayAnimMontage(dodgeAndAttack01);
}

void ABossApernia::ServerRPC_SpawnNiagaraDodgeAndSlash_Implementation()
{
    MulticastRPC_SpawnNiagaraDodgeAndSlash();
}

void ABossApernia::MulticastRPC_SpawnNiagaraDodgeAndSlash_Implementation()
{
    // 보스의 위치와 방향을 가져옴
    FVector bossLocation = GetActorLocation();
    FRotator bossRotation = GetActorRotation();
    FVector bossForwardVector = GetActorForwardVector();
    FVector bossRightVector = GetActorRightVector(); // 보스의 오른쪽 벡터를 가져옴

    // 스폰 위치를 보스의 로컬 공간에서의 오프셋으로 설정
    FVector offset = bossForwardVector * -100.0f;
    offset += bossRightVector * -300.0f; // 보스의 왼쪽으로 100만큼 이동
    offset.Y -= 200.0f;
    offset.Z += 100.0f;

    // 오프셋을 월드 공간으로 변환
    FVector spawnLocation = bossLocation + bossRotation.RotateVector(offset);

    // 파티클의 회전값을 설정 (보스의 방향을 기반으로)
    FRotator spawnRotation = bossRotation + FRotator(0.0f, -250.0f, 0.0f);

    // 나이아가라 시스템을 해당 위치와 회전으로 스폰
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), doubleSwingNA2, spawnLocation, spawnRotation + FRotator(0.0f, 180.0f, 0.0f), FVector(2.0f));
}

void ABossApernia::ServerRPC_DoubleSwingAttack_Implementation()
{
    MulticastRPC_DoubleSwingAttack();
}

void ABossApernia::MulticastRPC_DoubleSwingAttack_Implementation()
{
    PlayAnimMontage(doubleSwingAttack);
}

void ABossApernia::ServerRPC_SpawnNiagaraDoubleSwing01_Implementation()
{
    MulticastRPC_SpawnNiagaraDoubleSwing01();
}

void ABossApernia::MulticastRPC_SpawnNiagaraDoubleSwing01_Implementation()
{
    // 보스의 위치와 방향을 가져옴
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    // 파티클을 스폰할 위치 계산 (Z축을 100만큼 올림)
    FVector spawnLocation = bossLocation + bossForwardVector * 500.0f;
    spawnLocation.Z += 100.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), doubleSwingV2, spawnLocation, FRotator::ZeroRotator, FVector(4.0f));
}

void ABossApernia::ServerRPC_SpawnNiagaraDoubleSwing02_Implementation()
{
    MulticastRPC_SpawnNiagaraDoubleSwing02();
}

void ABossApernia::MulticastRPC_SpawnNiagaraDoubleSwing02_Implementation()
{
    // 보스의 위치와 방향을 가져옴
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    // 파티클을 스폰할 위치 계산 (Z축을 100만큼 올림)
    FVector spawnLocation = bossLocation + bossForwardVector * 200.0f;
    spawnLocation.Z += 100.0f;

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), doubleSwingV3, spawnLocation, FRotator::ZeroRotator, FVector(3.0f));
}

void ABossApernia::ServerRPC_DoubleSwingCameraShake_Implementation()
{
    MulticastRPC_DoubleSwingCameraShake();
}

void ABossApernia::MulticastRPC_DoubleSwingCameraShake_Implementation()
{
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    for (AActor* actor : foundCharacters)
    {
        APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
        if (player)
        {
            APlayerController* pc = player->GetController<APlayerController>();
            if (pc != nullptr)
            {
                UE_LOG(LogTemp, Warning, TEXT("Trying to shake camera!"));
                pc->ClientStartCameraShake(cameraShakeOBJ2);


            }
        }
    }
}

void ABossApernia::ServerRPC_DodgeRight_Implementation()
{
    MulticastRPC_DodgeRight();
}

void ABossApernia::MulticastRPC_DodgeRight_Implementation()
{
    PlayAnimMontage(dodge01);
}

void ABossApernia::ServerRPC_DodgeLeft_Implementation()
{
    MulticastRPC_DodgeLeft();
}

void ABossApernia::MulticastRPC_DodgeLeft_Implementation()
{
    PlayAnimMontage(dodge02);
}

void ABossApernia::ServerRPC_DodgeBack_Implementation()
{
    MulticastRPC_DodgeBack();
}

void ABossApernia::MulticastRPC_DodgeBack_Implementation()
{
    PlayAnimMontage(dodge03);
}

void ABossApernia::ServerRPC_StingAttack_Implementation()
{
    MulticastRPC_StingAttack();
}

void ABossApernia::MulticastRPC_StingAttack_Implementation()
{
    PlayAnimMontage(stingAttack01);
}

void ABossApernia::ServerRPC_SpawnNiagaraStingAttack_Implementation()
{
    MulticastRPC_SpawnNiagaraStingAttack();
}

void ABossApernia::MulticastRPC_SpawnNiagaraStingAttack_Implementation()
{
    // 보스의 위치와 방향을 가져옴
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();



    // 파티클을 스폰할 위치 계산
    FVector spawnLocation = bossLocation + bossForwardVector * 400.0f;


    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), stingAttackNiagara, spawnLocation, FRotator::ZeroRotator, FVector(1.5f));
}

void ABossApernia::ServerRPC_StingAttackCameraShake_Implementation()
{
    MulticastRPC_StingAttackCameraShake();
}

void ABossApernia::MulticastRPC_StingAttackCameraShake_Implementation()
{
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    for (AActor* actor : foundCharacters)
    {
        APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
        if (player)
        {
            APlayerController* pc = player->GetController<APlayerController>();
            if (pc != nullptr)
            {
                
                pc->ClientStartCameraShake(cameraShakeStingOBJ);


            }
        }
    }
}

void ABossApernia::ServerRPC_FarJumpAttack_Implementation()
{
    MulticastRPC_FarJumpAttack();
}

void ABossApernia::MulticastRPC_FarJumpAttack_Implementation()
{
    PlayAnimMontage(farStompAttack);
}

void ABossApernia::ServerRPC_SpawnNiagaraFarJumpAttack_Implementation()
{
    MulticastRPC_SpawnNiagaraFarJumpAttack();
}

void ABossApernia::MulticastRPC_SpawnNiagaraFarJumpAttack_Implementation()
{
    FVector bossLocation = GetActorLocation();
    FVector bossForwardVector = GetActorForwardVector();

    FVector bossGroundLocation = bossLocation - FVector(0.0f, 0.0f, GetSimpleCollisionHalfHeight());
    FVector spawnLocation = bossGroundLocation + bossForwardVector * 200.0f;

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), groundImpactParticle2, spawnLocation, FRotator::ZeroRotator, FVector(0.7f));
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), groundImpactNiagara2, spawnLocation, FRotator::ZeroRotator, FVector(0.5f));
}


void ABossApernia::ServerRPC_FarJumpAttackCameraShake_Implementation()
{
    MulticastRPC_FarJumpAttackCameraShake();
}

void ABossApernia::MulticastRPC_FarJumpAttackCameraShake_Implementation()
{
    TArray<AActor*> foundCharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APixelCodeCharacter::StaticClass(), foundCharacters);

    for (AActor* actor : foundCharacters)
    {
        APixelCodeCharacter* player = Cast<APixelCodeCharacter>(actor);
        if (player)
        {
            APlayerController* pc = player->GetController<APlayerController>();
            if (pc != nullptr)
            {

                pc->ClientStartCameraShake(cameraShakeFarStompAttackOBJ);


            }
        }
    }
}

void ABossApernia::ServerRPC_SwordFirstEquip_Implementation()
{
    MulticastRPC_SwordFirstEquip();
}

void ABossApernia::MulticastRPC_SwordFirstEquip_Implementation()
{
    PlayAnimMontage(swordEquipMT);
}

void ABossApernia::ServerRPC_FarJumpAttackSwordPositionSet_Implementation()
{
    MulticastRPC_FarJumpAttackSwordPositionSet();
}

void ABossApernia::MulticastRPC_FarJumpAttackSwordPositionSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(41.750762f, 266.377071, 22.241364));
    bossSwordComp->SetRelativeRotation(FRotator(-4.150309f, -5.576854f, 180.027317f));
}

void ABossApernia::ServerRPC_StingAttackSwordPositionSet_Implementation()
{
    MulticastRPC_StingAttackSwordPositionSet();
}

void ABossApernia::MulticastRPC_StingAttackSwordPositionSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
    bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
}

void ABossApernia::ServerRPC_FarJumpAttackSwordPositionReSet_Implementation()
{
    MulticastRPC_FarJumpAttackSwordPositionReSet();
}

void ABossApernia::MulticastRPC_FarJumpAttackSwordPositionReSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
    bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
}

void ABossApernia::ServerRPC_StingAttackSwordPositionReSet_Implementation()
{
    MulticastRPC_StingAttackSwordPositionReSet();
}

void ABossApernia::MulticastRPC_StingAttackSwordPositionReSet_Implementation()
{
    
    bossSwordComp->SetRelativeLocation(FVector(-21.312938, 82.751212, 0.209361));
    bossSwordComp->SetRelativeRotation(FRotator(2.126447, 153.892130, 190.052192));
}

void ABossApernia::ServerRPC_JumpAttack03SwordPositionSet_Implementation()
{
    MulticastRPC_JumpAttack03SwordPositionSet();
}

void ABossApernia::MulticastRPC_JumpAttack03SwordPositionSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(17.137708f, 57.508425f, 23.246429f));
    bossSwordComp->SetRelativeRotation(FRotator(28.852794f, 169.726741f, 183.362852f));
}

void ABossApernia::ServerRPC_JumpAttack03SwordPositionReSet_Implementation()
{
    MulticastRPC_JumpAttack03SwordPositionReSet();
}

void ABossApernia::MulticastRPC_JumpAttack03SwordPositionReSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
    bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
}

void ABossApernia::ServerRPC_SpawnJumpAttackSwordPositionSet_Implementation()
{
    MulticastRPC_SpawnJumpAttackSwordPositionSet();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackSwordPositionSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(67.829360f, 65.919485f, -5.596131f));
    bossSwordComp->SetRelativeRotation(FRotator(8.929752f, 199.055201f, 197.468674f));
}

void ABossApernia::ServerRPC_SpawnJumpAttackSwordPositionSet2_Implementation()
{
    MulticastRPC_SpawnJumpAttackSwordPositionSet2();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackSwordPositionSet2_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(-9.262842f, 91.403065f, -6.445372f));
    bossSwordComp->SetRelativeRotation(FRotator(70.003781f, 182.098563f, 218.299690f));
}

void ABossApernia::ServerRPC_SpawnJumpAttackSwordPositionReSet_Implementation()
{
    MulticastRPC_SpawnJumpAttackSwordPositionReSet();
}

void ABossApernia::MulticastRPC_SpawnJumpAttackSwordPositionReSet_Implementation()
{
    bossSwordComp->SetRelativeLocation(FVector(29.425722f, 55.060376f, 8.3646449f));
    bossSwordComp->SetRelativeRotation(FRotator(4.826905f, 1.306981f, 8.324931f));
}





void ABossApernia::ServerRPC_BossFallDown_Implementation(float Damage)
{
    MulticastRPC_BossFallDown(Damage);
}

void ABossApernia::MulticastRPC_BossFallDown_Implementation(float Damage)
{
    bossCurrentHP = bossCurrentHP - Damage;
    Player = Cast<APlayerOrganism>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

    if (Player->bBossGroggy == true)
    {
        BossFallDown();


        PlayAnimMontage(bossFallDownMT);
        if (ABossAIController* bossController = Cast<ABossAIController>(GetController()))
        {
            bossController->ClearFocus(EAIFocusPriority::Default);
            bossController->StopMovement();
            FRotator currentRotation = GetActorRotation();
            SetActorRotation(currentRotation);


            if (UBehaviorTreeComponent* BTComponent = bossController->FindComponentByClass<UBehaviorTreeComponent>())
            {
                savedBTComponent = BTComponent; // Save the BTComponent
                savedLocation = GetActorLocation(); // Save the current location
                BTComponent->StopTree(EBTStopMode::Safe);
            }
            GetWorldTimerManager().SetTimer(timerhandle_RepocessBehaviorTree, this, &ABossApernia::RepocessBehaviorTree, 6.0f, false);
            bBossAttackFallDownAttack = true;
        }
    }
    
}

void ABossApernia::ServerRPC_CounterPrecursor_Implementation()
{
    MulticastRPC_CounterPrecursor();
}

void ABossApernia::MulticastRPC_CounterPrecursor_Implementation()
{
    PlayAnimMontage(counterPrecursor);
    
    


}

void ABossApernia::ServerRPC_CounterPrecursorSpawnParticle_Implementation()
{
    MulticastRPC_CounterPrecursorSpawnParticle();
}

void ABossApernia::MulticastRPC_CounterPrecursorSpawnParticle_Implementation()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), counterImpactParticle, GetActorLocation(), GetActorRotation(), FVector(2.0f));
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), counterSound, GetActorLocation());
    //UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), counterHitNA2, GetActorLocation(), GetActorRotation(), FVector(1.0f));
}

void ABossApernia::ServerRPC_BossDie_Implementation()
{
    MulticastRPC_BossDie();
}

void ABossApernia::MulticastRPC_BossDie_Implementation()
{
    PlayAnimMontage(bossDie);
}

void ABossApernia::ServerRPC_BossTakeDamageWidgetSet_Implementation()
{
    widgetRandomValue = FMath::RandRange(1, 5);

    UUserWidget* UserWidget = damageWidgetComponentl->GetUserWidgetObject();
    if (UserWidget)
    {
        damageWidgetInstance = Cast<UDamageWidget>(UserWidget);
        // damageWidgetInstance->SetDamage();
        if (widgetRandomValue == 1)
        {
            damageAmount = FMath::RandRange(800, 900);
            //damageWidgetInstance->PlayDamageAnimation01();

        }
        else if (widgetRandomValue == 2)
        {
            damageAmount = FMath::RandRange(2100, 2200);
            //damageWidgetInstance->PlayDamageAnimation02();
        }
        else if (widgetRandomValue == 3)
        {
            damageAmount = FMath::RandRange(700, 800);
            // damageWidgetInstance->PlayDamageAnimation03();
        }
        else if (widgetRandomValue == 4)
        {
            damageAmount = FMath::RandRange(500, 1000);
            // damageWidgetInstance->PlayDamageAnimation04();
        }
        else
        {
            damageAmount = FMath::RandRange(1000, 1200);
            // damageWidgetInstance->PlayDamageAnimation05();
        }

        UE_LOG(LogTemp, Warning, TEXT("damageAmount : %d"), damageAmount);
    }



    MulticastRPC_BossTakeDamageWidgetSet(damageAmount);
}

void ABossApernia::MulticastRPC_BossTakeDamageWidgetSet_Implementation(int32 value2)
{
    UUserWidget* UserWidget = damageWidgetComponentl->GetUserWidgetObject();
    if (UserWidget)
    {
        damageWidgetInstance = Cast<UDamageWidget>(UserWidget);
        damageWidgetInstance->SetDamage();
        if (value2 >= 800 && value2 <= 900)
        {

            damageWidgetInstance->PlayDamageAnimation01(value2);
            bossCurrentHP = bossCurrentHP - value2;

        }
        else if (value2 >= 2100 && value2 <= 2200)
        {

            damageWidgetInstance->PlayDamageAnimation02(value2);
            bossCurrentHP = bossCurrentHP - value2;
        }
        else if (value2 >= 700 && value2 <= 800)
        {

            damageWidgetInstance->PlayDamageAnimation03(value2);
            bossCurrentHP = bossCurrentHP - value2;
        }
        else if (value2 >= 500 && value2 <= 1000)
        {

            damageWidgetInstance->PlayDamageAnimation04(value2);
            bossCurrentHP = bossCurrentHP - value2;
        }
        else
        {

            damageWidgetInstance->PlayDamageAnimation05(value2);
            bossCurrentHP = bossCurrentHP - value2;
        }

    }

   
}
