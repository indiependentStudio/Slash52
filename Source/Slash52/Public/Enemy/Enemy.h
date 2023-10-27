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
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

	virtual float TakeDamage(
		float DamageAmount,
		FDamageEvent const& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	virtual void Destroyed() override;

protected:
	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	virtual void BeginPlay() override;

	virtual void Die() override;

	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	// UF because this will be bound to a delegate
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarComponent;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, Category="Combat")
	double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="AI Navigation")
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 150.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPawnSensingComponent> PawnSensingComponent;

	/* 
	 * Navigation
	 */
	UPROPERTY()
	TObjectPtr<AAIController> EnemyController;

	// Current Patrol Target - temp exposed to BP to work on Enemy seeing Player
	UPROPERTY(EditInstanceOnly, Category="AI Navigation", BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	TObjectPtr<AActor> PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category="AI Navigation")
	TArray<TObjectPtr<AActor>> PatrolTargets;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	void CheckCombatTarget();
	void CheckPatrolTarget();

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> WeaponClass;
};
