// 2023, indiependent.studio. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SlashAnimInstance.generated.h"

enum class ECharacterState : uint8;
class UCharacterMovementComponent;
class ASlashCharacter;
/**
 * 
 */
UCLASS()
class SLASH52_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// replacing BP's Event Blueprint Initialize Animation
	virtual void NativeInitializeAnimation() override;

	// replacing BP's Blueprint Update Animation
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ASlashCharacter> SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category="Movement |")
	TObjectPtr<UCharacterMovementComponent> SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly,Category="Movement |")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly,Category="Movement |")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category="Movement | Character State")
	ECharacterState CharacterState;
};
