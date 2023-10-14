// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class SLASH52_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DrawDirectionalHitVectors(
		FVector Forward,
		FVector ToHit,
		double Theta,
		FVector CrossProduct,
		FString HitDirection
	);
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;
	
	virtual void BeginPlay() override;

	void Die();

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	
	/*
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName);
	FName ChooseRandomMontageSection(UAnimMontage* AnimMontage, int32& OutSectionNumberId);

private:
	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="VFX")
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarComponent;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category="Combat")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	double PatrolRadius = 200.f;

	/* 
	 * Navigation
	 */
	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;
	
	// Current Patrol Target
	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	void CheckCombatTarget();
	void CheckPatrolTarget();
};
