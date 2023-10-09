// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIHelpers.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SlashCharacter.generated.h"

class AWeapon;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class AItem;

UCLASS()
class SLASH52_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> EKeyPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> AttackAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	void Attack();

	/*
	 * Play montage functions
	 */
	void PlayAttackMontage();
	void PlayEquipMontage(const FName& SectionName);
	static FName ChooseRandomMontageSection(UAnimMontage* AnimMontage);

	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> ViewCamera;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Hair;
	
	UPROPERTY(VisibleAnywhere, Category="Hair")
	TObjectPtr<UGroomComponent> Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AItem> OverlappingItem;

	UPROPERTY(VisibleInstanceOnly, Category="Weapon")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="Weapon", meta=(AllowPrivateAccess="true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	TObjectPtr<AWeapon> EquippedWeapon;

	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> EquipMontage;

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};

