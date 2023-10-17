// 2023, indiependent.studio. All rights reserved.


#include "Enemy/Enemy.h"

#include <string>

#include "AIController.h"
#include "NavigationPath.h"
#include "Characters/SlashCharacter.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Slash52/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	HealthBarComponent = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensingComponent->SightRadius = 2000.f;
	PawnSensingComponent->SetPeripheralVisionAngle(55.f);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (AttributeComponent && HealthBarComponent)
	{
		HealthBarComponent->SetHealthPercent(1.f);
		HealthBarComponent->SetVisibility(false);
	}

	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);

	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}
}

void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		int32 SectionNum;
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection(ChooseRandomMontageSection(DeathMontage, SectionNum),
		                                    DeathMontage);

		switch (SectionNum)
		{
		case 1:
			DeathPose = EDeathPose::EDP_Death1;
			break;
		case 2:
			DeathPose = EDeathPose::EDP_Death2;
			break;
		case 3:
			DeathPose = EDeathPose::EDP_Death3;
			break;
		case 4:
			DeathPose = EDeathPose::EDP_Death4;
			break;
		case 5:
			DeathPose = EDeathPose::EDP_Death5;
			break;
		case 6:
			DeathPose = EDeathPose::EDP_Death6;
			break;
		default:
			DeathPose = EDeathPose::EDP_Death1;
			break;
		}
	}

	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->DestroyComponent();
	SetLifeSpan(6.f);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) { return false; }
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Length();
	DRAW_SPHERE_SINGLE_FRAME(GetActorLocation());
	DRAW_SPHERE_SINGLE_FRAME(Target->GetActorLocation());
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);

	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemy::ChoosePatrolTarget()
{
	// make sure we don't keep randomly selected the TargetPoint we're already at
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		AActor* Target = ValidTargets[TargetSelection];
		return Target;
	}
	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if (EnemyState == EEnemyState::EES_Chasing) { return; }

	if (SeenPawn->ActorHasTag(FName("SlashCharacter")))
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f; // expose to BP
		CombatTarget = SeenPawn;
		
		if (EnemyState != EEnemyState::EES_Attacking)
		{
			EnemyState = EEnemyState::EES_Chasing;
			MoveToTarget(CombatTarget);
			UE_LOG(LogTemp, Warning, TEXT("Player seen."));
		}
		
	}
}

void AEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

FName AEnemy::ChooseRandomMontageSection(UAnimMontage* AnimMontage, int32& OutSectionNumberId)
{
	if (AnimMontage)
	{
		OutSectionNumberId = FMath::RandRange(1, AnimMontage->CompositeSections.Num());
		FString SectionNameAsString = FString("Death").Append(FString::FromInt(OutSectionNumberId));
		return FName(SectionNameAsString);
	}
	return FName("Default");
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

// If we move away from an Enemy, hide the HealthBar
void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius))
	{
		CombatTarget = nullptr;
		if (HealthBarComponent)
		{
			HealthBarComponent->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToTarget(PatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("Lose Interest"));
	}
	else if (!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing)
	{
		// outside Attack Range, chase character
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToTarget(CombatTarget);
		UE_LOG(LogTemp, Warning, TEXT("Chase Player"));
	}
	else if (InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking)
	{
		// inside Attack Range
		EnemyState = EEnemyState::EES_Attacking;
		// TODO: Attack Montage
		UE_LOG(LogTemp, Warning, TEXT("Attack!"));
	}
}

// If we're within range of a PatrolTarget, pick a new one, wait, then move to it
void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, FMath::RandRange(3.f, 9.f));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::DrawDirectionalHitVectors(const FVector Forward, const FVector ToHit, double Theta,
                                       const FVector CrossProduct, FString HitDirection)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Green,
		                                 FString::Printf(TEXT("Theta: %f %s"), Theta, *HitDirection));
	}

	UKismetSystemLibrary::DrawDebugArrow(this,
	                                     GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red,
	                                     60.f);

	UKismetSystemLibrary::DrawDebugArrow(this,
	                                     GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green,
	                                     60.f);

	UKismetSystemLibrary::DrawDebugArrow(this,
	                                     GetActorLocation(), GetActorLocation() + CrossProduct * 60.f, 5.f,
	                                     FColor::Blue, 60.f);
}

void AEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower impact point to enemy's Actor Location Z (so when we draw our debug arrows they are parallel to the ground)
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward . ToHit = |Forward| |ToHit| * cos(theta)
	// Forward = 1, ToHit = 1, so Forward . ToHit = cos(theta) (ie dot product)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// Forward x ToHit = |Forward| |ToHit| * sin(theta) * n
	// If CP points down, theta is -ve
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FString HitDirection = "From Back";
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
		HitDirection = "From Front";
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
		HitDirection = "From Left";
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
		HitDirection = "From Right";
	}
	PlayHitReactMontage(Section);

	// DrawDirectionalHitVectors(Forward, ToHit, Theta, CrossProduct, HitDirection);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	// DRAW_SPHERE_COLOR(ImpactPoint, FColor::Yellow);

	if (HealthBarComponent && !HealthBarComponent->IsVisible())
	{
		HealthBarComponent->SetVisibility(true);
	}

	if (AttributeComponent && AttributeComponent->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
	}
	else if (AttributeComponent)
	{
		Die();
	}

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
	}
}

// Called by Weapon's ApplyDamage
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	if (AttributeComponent && HealthBarComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
		HealthBarComponent->SetHealthPercent(AttributeComponent->GetHealthPercent());
	}

	CombatTarget = EventInstigator->GetPawn();

	return DamageAmount;
}
