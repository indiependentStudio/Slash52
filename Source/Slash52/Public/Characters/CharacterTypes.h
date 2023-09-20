#pragma once

UENUM(BlueprintType)
// Enumeration for Weapon equip states
enum class ECharacterState : uint8
{
	ECS_Unequipped UMETA(DisplayName="Weapon not equipped"),
	ECS_EquippedOneHandedWeapon UMETA(DisplayName="Equipped 1-handed weapon"),
	ECS_EquippedTwoHandedWeapon UMETA(DisplayName="Equipped 2-handed weapon")
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName="Unoccupied"),
	EAS_Attacking UMETA(DisplayName="Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName="Equipping Weapon")
};

