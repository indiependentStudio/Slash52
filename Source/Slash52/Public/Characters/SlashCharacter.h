// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class AItem;

UCLASS()
class SLASH52_API ASlashCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	/* Input */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void EKeyPressed();
	
	/* Play montages */
	void PlayEquipMontage(const FName& SectionName);

	/* Attacking and Arming */
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;

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

private:
	bool CanDisarm();
	bool CanArm();
	void Disarm();
	void Arm();
	void EquipWeapon(AWeapon* Weapon);

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
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

	/*
	 * Animation montages
	 */
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> EquipMontage;

	

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
