// 2023, indiependent.studio. All rights reserved.


#include "Characters/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter) DirectionalHitReact(Hitter->GetActorLocation());
	else if (AttributeComponent) Die();
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void ABaseCharacter::Attack()
{
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

int32 ABaseCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::Die()
{
}

bool ABaseCharacter::IsAlive()
{
	return AttributeComponent && AttributeComponent->IsAlive();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::DrawDirectionalHitVectors(FVector Forward, FVector ToHit, double Theta, FVector CrossProduct,
                                               FString HitDirection)
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

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
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

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, ImpactPoint);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (AttributeComponent)
	{
		AttributeComponent->ReceiveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* AnimMontage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimMontage)
	{
		AnimInstance->Montage_Play(AnimMontage);
		AnimInstance->Montage_JumpToSection(SectionName, AnimMontage);
	}
}


