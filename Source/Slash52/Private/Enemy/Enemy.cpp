// 2023, indiependent.studio. All rights reserved.


#include "Enemy/Enemy.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Slash52/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
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

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	const double cosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(cosTheta);
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

	DirectionalHitReact(ImpactPoint);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}

	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
	}
}