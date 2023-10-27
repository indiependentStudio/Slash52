// 2023, indiependent.studio. All rights reserved.


#include "Characters/BaseCharacter.h"

#include "Components/AttributeComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

FName ABaseCharacter::ChooseRandomMontageSection(UAnimMontage* AnimMontage)
{
	if (AnimMontage)
	{
		const int32 SectionNumberId = FMath::RandRange(1, AnimMontage->CompositeSections.Num());
		const FString SectionNameAsString = FString("Attack").Append(FString::FromInt(SectionNumberId));
		return FName(SectionNameAsString);
	}
	return FName("Default");
}

FName ABaseCharacter::ChooseRandomMontageSection(UAnimMontage* AnimMontage, int32& OutSectionNumberId)
{
	if (AnimMontage)
	{
		OutSectionNumberId = FMath::RandRange(1, AnimMontage->CompositeSections.Num());
		FString SectionNameAsString = FString("Death").Append(FString::FromInt(OutSectionNumberId));
		return FName(SectionNameAsString);
	}
	return FName("Default");
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
