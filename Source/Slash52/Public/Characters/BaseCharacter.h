// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

class UAttributeComponent;
class AWeapon;

UCLASS()
class SLASH52_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
	void DrawDirectionalHitVectors(
		FVector Forward,
		FVector ToHit,
		double Theta,
		FVector CrossProduct,
		FString HitDirection
	);

protected:
	virtual void BeginPlay() override;

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* </IHitInterface> */

	virtual void Attack();
	virtual bool CanAttack();
	virtual void HandleDamage(float DamageAmount);
	virtual void Die();
	bool IsAlive();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	
	/*
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TObjectPtr<AWeapon> EquippedWeapon;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

private:
	/*
	 * Play montage functions
	 */
	void PlayMontageSection(UAnimMontage* AnimMontage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UParticleSystem> HitParticles;

	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FName> DeathMontageSections;
};
