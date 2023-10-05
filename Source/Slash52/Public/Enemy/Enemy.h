// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

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
	void DrawDirectionalHitVectors(FVector Forward, FVector ToHit, double Theta, FVector CrossProduct,
	                               FString HitDirection);
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;

protected:
	virtual void BeginPlay() override;

	/*
	 * Play montage functions
	 */
	void PlayHitReactMontage(const FName& SectionName);
	
private:
	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Sounds")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category="VFX")
	TObjectPtr<UParticleSystem> HitParticles;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthBarComponent> HealthBarWidget;

};
