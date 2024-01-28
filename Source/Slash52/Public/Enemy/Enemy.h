// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UPawnSensingComponent;
class AAIController;
class UHealthBarComponent;
class UAttributeComponent;

UCLASS()
class SLASH52_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/* <AActor> */
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void Destroyed() override;
	/* </AActor> */

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* </IHitInterface> */

protected:
	/* <AActor> */
	virtual void BeginPlay() override;
	/* </AActor> */

	/* <ABaseCharacter> */
	virtual bool CanAttack() override;
	virtual void Attack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void AttackEnd() override;
	virtual void Die() override;
	virtual int32 PlayDeathMontage() override;
	/* </ABaseCharacter> */

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	
	// UF because this callback will be bound to a delegate
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	UPROPERTY(EditAnywhere, Category="AI | Combat")
	float DeathLifeSpan = 8.f;

	UPROPERTY(BlueprintReadOnly, Category="AI | Combat")
	TObjectPtr<AActor> CombatTarget;

	// I suddenly can't see Patrol Targets on my Paladin instances, so moving to Protected
	// Current Patrol Target - temp exposed to BP Graph to work on Enemy seeing Player
	UPROPERTY(EditInstanceOnly, Category="AI | Patrol", BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI | Patrol")
	TArray<TObjectPtr<AActor>> PatrolTargets;


private:
	/* AI Behavior */
	void InitializeEnemy();
	void CheckCombatTarget();
	void CheckPatrolTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	bool IsOutsideCombatRadius();
	void ChaseTarget();
	bool IsOutsideAttackRadius();
	bool IsChasing();
	bool IsInsideAttackRadius();
	bool IsAttacking();
	void ClearPatrolTimer();
	bool IsDead();
	bool IsEngaged();
	void StartAttackTimer();
	void ClearAttackTimer();
	void SpawnDefaultWeapon();
	
	FTimerHandle PatrolTimer;
	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	float AttackMinDelay = 0.5f;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	float AttackMaxDelay = 1.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	double CombatRadius = 1500.f;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category="AI | Combat")
	float ChasingSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category="AI | Navigation")
	double PatrolRadius = 200.f;

	
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AWeapon> WeaponClass;
};
