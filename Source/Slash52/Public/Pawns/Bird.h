// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Bird.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UFloatingPawnMovement;
class UInputAction;
class UCapsuleComponent;
class UInputMappingContext;

UCLASS()
class SLASH52_API ABird : public APawn
{
	GENERATED_BODY()

public:
	ABird();
		virtual void Tick(float DeltaTime) override;
	
		// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	protected:
		virtual void BeginPlay() override;
	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
		TObjectPtr<UInputMappingContext> BirdMappingContext;
	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
		TObjectPtr<UInputAction> MoveAction;
	
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
		TObjectPtr<UInputAction> LookAction;
	
		void Move(const FInputActionValue& Value);
		void Look(const FInputActionValue& Value);
	
	private:
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCapsuleComponent> Capsule;
	
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<USkeletalMeshComponent> BirdMesh;
	
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<UFloatingPawnMovement> FloatingPawnMovement;
	
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<USpringArmComponent> SpringArm;
	
		UPROPERTY(VisibleAnywhere)
		TObjectPtr<UCameraComponent> ViewCamera;
};